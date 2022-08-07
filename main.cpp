#include <iostream>
#include <fstream>
#include <float.h>
#include <memory>
#include "rtweekend.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "matte.h"
#include "dielectric.h"
#include "metal.h"

// takes in a ray and a list of hittable objects and returns color of the object that was hit
color ray_color(const ray& r, const hittable& world, int depth) {
  if (depth <= 0)
    return color(0,0,0);

  hit_record rec;
  ray reflected_ray;

  if (world.hit(r, 0.001, DBL_MAX, rec)) {
    reflected_ray = rec.material_ptr->scatter(r, rec); 
    return rec.material_ptr->albedo * ray_color(reflected_ray, world, depth-1);
  }
  return color(1,1,1);
}

int main() {

  /* Setup image and objects */
  camera cam(point3(-1,1,0), 45, -45, 16.0/9.0, 90);
  const int image_width = 400;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  hittable_list world;

  auto material_ground = std::make_shared<matte>(color(0.8, 0.8, 0.0));
  auto material_center = std::make_shared<matte>(color(0.4));
  auto material_left   = std::make_shared<metal>(color(0.8, 0.8, 0.8), 0.7);
  auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.5);
  auto material_dielectric = std::make_shared<dielectric>(1.42);

  auto l = std::cos(3.14159/4);

  world.add(std::make_shared<sphere>(point3( 0.0, -100.5*l, -1.0), 100.0*l, material_ground));
  world.add(std::make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.45*l, material_center));
  world.add(std::make_shared<sphere>(point3(-l,    0.0, -1.0),   0.5*l, material_left));
  world.add(std::make_shared<sphere>(point3( l,    0.0, -1.0),   0.5*l, material_right));

  const int samples_per_pixel = 100;
  const int bounce_depth = 50;

  /* Setup PPM file */
  std::ofstream img;
  img.open("render.ppm");
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  auto start_time = Time::now();
  hit_record rec;
  for (int i = image_height; i > 0; --i) { // must go from top to bottom as PPM file starts in top left corner, not bottom
    std::cout << "\rScanlines remaining: " << i-1 << ' ' << std::flush;
    for (int j = 0; j < image_width; ++j) {
      color sum(0,0,0);
      for (int k = 0; k < samples_per_pixel; ++k) {
        double u = (j+random_double()) / image_width;
        double v = (i+random_double()) / image_height;
        ray r = cam.get_ray(u, v);
        sum += ray_color(r, world, bounce_depth);
      }
     write_color(img, sqrt((sum/samples_per_pixel))); // sqrt for gamma correction
    }
  }

  print_render_time(Time::now() - start_time, std::cout, 3);

  img.close();
  return 0;

}
