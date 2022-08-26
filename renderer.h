#ifndef RENDERER_H
#define RENDERER_H

#include <thread>
#include <cmath>
#include <iostream>
#include <fstream>
#include <limits>
#include <float.h>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "material.h"
#include "hittable_list.h"
#include "rtweekend.h"

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
  	renderer() { frame_count = 0; }

  	void render_to_file(const std::string filename);
    void render_shifting_focus(point3 startpoint, point3 endpoint, const video_params& vp);
    void render_spinning_circle(const spinning_circle_params& scp);
    void render_straight_line(point3 endpoint, const video_params& vp);

  private:
  	color ray_color(const ray& r, int depth);
  	void render_to_mem(pixel* image);
    int frame_count;

  public:
    hittable_list world;
    camera cam;
    int image_width;
    int image_height;
    int samples_per_pixel;
    int bounce_depth;
    int core_count;
};

/* Takes in a ray and a list of hittable objects and returns color of the object that was hit */
color renderer::ray_color(const ray& r, int depth) {
  // If bounce depth has been reached, return black color
  if (depth <= 0) return color(0,0,0);

  hit_record rec;
  ray reflected_ray;

  if (world.hit(r, 0.001, DBL_MAX, rec)) {
    reflected_ray = rec.material_ptr->scatter(r, rec); 
    return rec.material_ptr->albedo * ray_color(reflected_ray, depth-1);
  }
  return color(1,1,1);
}

/* Renders image into an array of pixels in memory (splits samples_per_pixel across # of threads) */
void renderer::render_to_mem(pixel* image) {
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
      image[(image_height-i)*image_width + j] = sqrt(sum/divided_spp);  // sqrt for gamma correction
    }
  }
}

/* Renders image into a file (multithreaded) */
void renderer::render_to_file(const std::string filename) {

  /* Create output image stream */
  std::ofstream img;
  img.open(filename);
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  auto start_time = Time::now();
 
  // create an array of images (pixel arrays), one for each CPU core
  pixel* images[core_count];

  // create an array of threads
  std::thread threads[core_count-1];

  // initialize images (set the size of the arrays)
  for (int i = 0; i < core_count; ++i)
    images[i] = new pixel[image_height*image_width];

  // launch core_count-1 threads for image rendering
  for (int i = 0; i < core_count-1; ++i)
    threads[i] = std::thread(&renderer::render_to_mem, this, images[i]);

  // render image on this thread
  render_to_mem(images[core_count-1]);

  // join launched threads
  for (int i = 0; i < core_count-1; ++i)
    threads[i].join();

  // average all the images
  for (int i = 0; i < image_height*image_width; ++i) {
    color sum;
    for (int j = 0; j < core_count; ++j)
      sum += images[j][i];
    write_color(img, sum/core_count);
  }

  // delete pixel arrays (images) out of memory
  for (int i = 0; i < core_count; ++i)
    delete images[i];

  print_render_time(Time::now() - start_time, std::cout, 3);

  img.close();
}

/* Renders frames of video where focus smoothly shifts from cam current origin to endpoint throughout the video */
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

/* Renders frames of video in which camera circles counter-clockwise around a central point (in plane specified by e1 & e2) */
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