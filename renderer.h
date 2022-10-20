#ifndef RENDERER_H
#define RENDERER_H

#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <memory>
#include <float.h>
#include <SDL2/SDL.h>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "rtweekend.h"
#include "image.h"

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
    color ray_color(const ray& r, int depth) const;
    void st_render_to_mem(image img) const;
    image mt_render_to_mem(image_list images) const;
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

/* Takes in a ray and bounce depth and returns color of the object that was hit */
color renderer::ray_color(const ray& r, int depth) const {
  // If bounce depth has been reached, return black color
  if (depth < 0) return color(0,0,0);

  hit_record rec;
  ray reflected_ray;

  if (world.hit(r, 0.001, DBL_MAX, rec)) {
    reflected_ray = rec.material_ptr->scatter(r, rec); 
    return rec.material_ptr->albedo * ray_color(reflected_ray, depth-1);
  }
  return color(1,1,1);
}

/* Single-threaded render to memory location passed in as argument */
void renderer::st_render_to_mem(image img) const {

    // Split the render across all cores
    int divided_spp = samples_per_pixel/core_count;

    for (int i = image_height; i > 0; --i) {
        for (int j = 0; j < image_width; ++j) {
            color sum;
            for (int k = 0; k < divided_spp; ++k) {
                double u = (j+random_double()) / image_width;
                double v = (i+random_double()) / image_height;
                ray r = cam.get_ray(u, v);
                sum += ray_color(r, bounce_depth);
            }
            img[(image_height-i)*image_width + j] = sqrt(sum/divided_spp);  // sqrt for gamma correction
        }
    }
}

/* Multi-threaded render to memory; takes in images array to render to; returns pointer to final, averaged image */
image renderer::mt_render_to_mem(image_list images) const {

    // create an array of threads
    std::thread threads[core_count];

    // launch as many threads as CPU cores, rendering one image on each thread
    for (int i = 0; i < core_count; ++i)
        threads[i] = std::thread(&renderer::st_render_to_mem, this, images[i]);

    // join launched threads
    for (int i = 0; i < core_count; ++i)
        threads[i].join();

    // Average images and return
    return average_images(images, core_count, image_width, image_height);
}

/* Renders image into a file (multithreaded) */
void renderer::render_to_file(const std::string filename) const {

    // Create output image stream
    std::ofstream PPM;
    PPM.open(filename);
    PPM << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    // Render start time
    auto start_time = Time::now();

    // Allocate images list
    image_list partial_renders = alloc_images(image_width, image_height, core_count);

    // Render image
    image final_render = mt_render_to_mem(partial_renders);
    print_render_time(Time::now() - start_time, std::cout, 3);

    // Delete list of images out of memory
    delete_images(partial_renders, core_count);

    // Write pixel values from memory into file
    for (int i = 0; i < image_width*image_height; ++i)
        write_color(PPM, final_render[i]);

    // Deallocate rendered image out of memory
    delete [] final_render;

    PPM.close();
}

/* Renders scene and shows it in a program window */
void renderer::render_to_window() const {

    // Create window and renderer
    SDL_Window* window = SDL_CreateWindow( "Raytracing in One Weekend++", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, image_width, image_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Texture* buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, image_width, image_height);

    Uint32* pixels = NULL;  // An elaborate setup (that I don't understand) with the (void**) below that gives us access to the pixels array only during the texture being locked
    int pitch = 4*image_width;
    // Lock texture so that we can edit the raw pixels array
    SDL_LockTexture(buffer, NULL, (void**) &pixels, &pitch);

    // Render scene to memory
    image_list partial_renders = alloc_images(image_width, image_height, core_count);
    auto start_time = Time::now();
    image final_render = mt_render_to_mem(partial_renders);
    print_render_time(Time::now() - start_time, std::cout, 3);
    delete_images(partial_renders, core_count);

    // Edit pixels array of texture
    for (int j = 0; j < image_height*image_width; ++j)
      pixels[j] = convert_to_ARGB8888(final_render[j]);

    // Deallocate rendered image out of memory
    delete [] final_render;

    // Unlock texture as we're now done editing the pixels
    SDL_UnlockTexture(buffer);

    // Copy texture to renderer
    SDL_RenderCopy(renderer, buffer, NULL, NULL);

    // Refresh renderer
    SDL_RenderPresent(renderer);

    // Rendering is done, thus, deallocate texture & renderer
    SDL_DestroyTexture(buffer);
    SDL_DestroyRenderer(renderer);

    //Hack to get window to stay up
    SDL_Event e;
    bool quit = false;
    while(quit == false)
    {
      while(SDL_PollEvent( &e ))
      {
          if(e.type == SDL_QUIT)
              quit = true;
      }
    }

    // Deallocate window
    SDL_DestroyWindow(window);

    // Qut all SDL subsystems
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