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

color ray_color(const ray& r, const hittable& world, int depth) {
  if (depth <= 0)
    return color(0,0,0);

  hit_record rec;
  if (world.hit(r, 0, DBL_MAX, rec)) {
    point3 target = rec.p + rec.normal + random_in_unit_sphere();
    ray bounce = ray(rec.p, target - rec.p);
    return 0.5*ray_color(bounce, world, depth-1);
  } else return color(1,1,1);
}

int main() {

  /* Setup image and objects */
  camera cam;
  const int image_width = 1920;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(0,0,-1), 0.5));
  world.add(std::make_shared<sphere>(point3(0,-100.5,-1), 100));
  const int samples_per_pixel = 100;

  /* Setup PPM file */
  std::ofstream img;
  img.open("render.ppm");
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  hit_record rec;
  for (int i = image_height; i > 0; --i) { // must go from top to bottom as PPM file starts in top left corner, not bottom
    std::cerr << "\rScanlines remaining: " << i << ' ' << std::flush;
    for (int j = 0; j < image_width; ++j) {
      color sum(0,0,0);
      for (int k = 0; k < samples_per_pixel; ++k) {
        double u = (j+random_double()) / image_width;
        double v = (i+random_double()) / image_height;
        ray r = cam.get_ray(u, v);
        sum += ray_color(r, world, 50);
      }
     write_color(img, sum/samples_per_pixel); 
    }
  }

  img.close();
  return 0;
}
