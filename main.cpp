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

color ray_color(const ray& r, const hittable& world, int depth) {
  if (depth <= 0)
    return color(0,0,0);

  hit_record rec;
  ray reflected_ray;

  if (world.hit(r, 0.001, DBL_MAX, rec)) {
    reflected_ray = rec.material_ptr->scatter(r, rec); 
    return rec.material_ptr->get_albedo() * ray_color(reflected_ray, world, depth-1);
  }
  return color(1,1,1);
}

int main() {

  /* Setup image and objects */
  camera cam;
  const int image_width = 1280;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  hittable_list world;

  auto matte_material = std::make_shared<matte>(color(0.5));

  world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5, matte_material));
  world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100, matte_material));
  
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
