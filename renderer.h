#ifndef RENDERER_H
#define RENDERER_H

#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
#include <vector>
#include <float.h>
#include <SDL2/SDL.h>
#include <chrono>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "rtweekend.h"

using namespace std::chrono_literals;

struct video_params{
  int seconds;
  int fps;
  video_params(int s, int f) : seconds(s), fps(f) {} 
};

struct spinning_circle_params {
  vec3 e1;
  vec3 e2;
  point3 center;
  double radians;
  video_params vp;
};

class renderer {
  public:
    renderer() {
      frame_count = 0;
    }

    void render_to_file(const std::string filename) const;
    void render_to_window() const;
    void render_shifting_focus(point3 startpoint, point3 endpoint, const video_params& vp);
    void render_spinning_circle(const spinning_circle_params& scp);
    void render_straight_line(point3 endpoint, const video_params& vp);

  private:
    pixel ray_color(const ray& r, int depth) const;
    void st_render_to_mem(std::vector<Uint32>& pixels, bool* KILL) const;
    void mt_render_to_mem(std::vector<Uint32>& pixels, bool* RENDER_DONE, bool* KILL) const;
    int frame_count;

  public:  // perhaps make a bunch of these private and set them in the constructor
    hittable_list world;
    camera cam;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int bounce_depth;
    int core_count;
};

/* Takes in a ray and bounce depth and returns RGB color of the object that was hit */
pixel renderer::ray_color(const ray& r, int depth) const {
    // If bounce depth has been reached, return black color
    if (depth < 0) return color(0,0,0);

    hit_record rec;
    ray reflected_ray;

    if (world.hit(r, 0.001, DBL_MAX, rec)) {
        reflected_ray = rec.material_ptr->scatter(r, rec);
        return rec.material_ptr->albedo * ray_color(reflected_ray, depth-1);
    }
    return pixel(1,1,1);
}

/* Single-threaded render to memory location passed in. Adds final pixel divided by core count to each output pixel. */
void renderer::st_render_to_mem(std::vector<Uint32>& pixels, bool* KILL) const {

    // Split the render across all cores
    int divided_spp = samples_per_pixel/core_count;

    for (int i = 0; i < image_height; ++i) {
        for (int j = 0; j < image_width; ++j) {
            if (KILL != nullptr) if (*KILL == true) goto done;
            color sum;
            for (int k = 0; k < divided_spp; ++k) {
                double u = (j+random_double()) / image_width;
                double v = (i+random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                sum += ray_color(r, bounce_depth);
            }
            pixel final = sqrt(sum/divided_spp);   // sqrt for gamma correction
            pixel partial_avg = final / core_count;
            pixels[((image_height-i)*image_width)+j] += convert_to_ARGB8888(partial_avg);
        }
    }
    done: {};
}

/* Multi-threaded render to memory location passed in */
void renderer::mt_render_to_mem(std::vector<Uint32>& pixels, bool* RENDER_DONE, bool* KILL) const {

    // create an array of threads
    std::thread threads[core_count];

    // counter for st render threads completion
    int render_completion_count = 0;

    // launch as many threads as CPU cores, rendering one image on each thread
    for (int i = 0; i < core_count; ++i)
        threads[i] = std::thread(&renderer::st_render_to_mem, this, std::ref(pixels), KILL);

    // Wait for all threads to finish their renders
    for (int i = 0; i < core_count; ++i)
        threads[i].join();

    // if rendering to window, wait a small amount to allow final pixels to get drawn onto screen
    if (RENDER_DONE != nullptr && KILL != nullptr)  // make sure function is not being called from render_to_file() (in which case, never add delay)
        if (*KILL != true)  // We only want to add delay after scene has finished rendering; if it has not (window close command issued), do not add delay
            std::this_thread::sleep_for(60ms);

    if(RENDER_DONE != nullptr) *RENDER_DONE = true;
}

/* Renders image into a file (multithreaded) */
void renderer::render_to_file(const std::string filename) const {

    // Create output file stream
    std::ofstream PPM;
    PPM.open(filename);
    PPM << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Allocate new image
    std::vector<Uint32> pixels(image_width*image_height, 0);

    // Render into memory
    auto start_time = Time::now();
    mt_render_to_mem(pixels, nullptr, nullptr);
    print_render_time(Time::now() - start_time, std::cout, 3);

    // Write pixel values from memory into file
    for (int i = 0; i < image_width*image_height; ++i)
        write_ARGB8888_PPM(PPM, pixels[i]);

    // Close output file stream
    PPM.close();
}

/* Renders scene and shows it in a program window */
void renderer::render_to_window() const {

    // Create window and renderer
    SDL_Init( SDL_INIT_EVERYTHING );
    SDL_Window* window = SDL_CreateWindow( "Raytracing in One Weekend++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image_width, image_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* sdl_renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );  // use linear filtering for scaling

    // Create texture and allocate space in memory for image
    SDL_Texture* texture = SDL_CreateTexture(sdl_renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);
    std::vector<Uint32> pixels(image_width*image_height, 0);

    // Launch scene render on a separate thread
    bool RENDER_DONE = false;
    bool KILL = false;
    auto start_time = Time::now();
    std::thread headless_render(&renderer::mt_render_to_mem, this, std::ref(pixels), &RENDER_DONE, &KILL);

    // Main SDL window loop
    bool running = true;
    bool timer_done = false;
    while (running) {
        if (RENDER_DONE && !timer_done) {
            print_render_time(Time::now() - start_time, std::cout, 3);
            timer_done = true;
        }

        // SDL event handling
        SDL_Event e;
        while(SDL_PollEvent(&e)) { // return 1 if there is a pending event, otherwise 0 (loop doesn't run)
            if (e.type == SDL_QUIT) {
                if (RENDER_DONE == false) KILL = true;  // send kill command to threads only if render is still in progress
                running = false;
                break;
            }
        }

        if (RENDER_DONE == false) {  // Only continue copying image from memory into texture if render is in progress
            // Copy pixels from memory into the SDL texture
            Uint32* locked_pixels = nullptr;
            int pitch = image_width*4;
            SDL_LockTexture( texture, nullptr, reinterpret_cast< void** >( &locked_pixels ), &pitch );
            std::copy_n(pixels.data(), pixels.size(), locked_pixels);
            SDL_UnlockTexture(texture);

            // Copy texture to renderer
            SDL_RenderCopy(sdl_renderer, texture, nullptr, nullptr);
            // Update screen
            SDL_RenderPresent(sdl_renderer);
        }
    }

    headless_render.join();

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(sdl_renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

/* Renders frames of video where focus smoothly shifts from given startpoint to endpoint throughout the video */
void renderer::render_shifting_focus(point3 startpoint, point3 endpoint, const video_params& vp) {
    ray focus_line = ray(startpoint, endpoint-startpoint);
    int total_frames = vp.fps*vp.seconds;

    for (int curr_frame = 0; curr_frame < total_frames; ++curr_frame) {
        double progress = (((double)(curr_frame))/total_frames);
        cam.focus(focus_line.at(progress));
        render_to_file("output/" + std::to_string(frame_count + curr_frame) + ".ppm");
    }
    frame_count += total_frames;
}

/* Renders video frames of camera circling counter-clockwise around a central point (in plane specified by e1 & e2) */
void renderer::render_spinning_circle(const spinning_circle_params& scp) {
    vec3 up = cross(scp.e1, scp.e2);
    vec3 r = cam.origin - scp.center;
    vec3 x_hat = unit_vector(r);
    vec3 y_hat = unit_vector(cross(x_hat, -up));

    double radius = r.length();
    int total_frames = scp.vp.fps*scp.vp.seconds;

    for (int curr_frame = 0; curr_frame < total_frames; ++curr_frame) {
        double circle_prog = ((double)curr_frame)/total_frames;
        double angle = circle_prog*(scp.radians);
        cam.orient(scp.center + (radius*std::cos(angle)*x_hat + radius*std::sin(angle)*y_hat), scp.center, up);
        render_to_file("output/" + std::to_string(frame_count + curr_frame) + ".ppm");
    }
    frame_count += total_frames;
}

/* Renders video frames of camera moving in a straight line from current origin to given endpoint */
void renderer::render_straight_line(point3 endpoint, const video_params& vp) {
    vec3 path_vector = endpoint - cam.origin;
    ray path_ray = ray(cam.origin, path_vector);
    double path_length = path_vector.length();

    int total_frames = vp.fps*vp.seconds;
    double pan_amount_per_frame = path_length/total_frames;

    for (int curr_frame = 0; curr_frame < total_frames; ++curr_frame) {
        cam.pan(path_vector, pan_amount_per_frame);
        render_to_file("output/" + std::to_string(frame_count + curr_frame) + ".ppm");
    }
    frame_count += total_frames;
}

#endif