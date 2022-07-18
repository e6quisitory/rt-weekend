#include <iostream>
#include <fstream>
#include <float.h>
#include <memory>
#include "rtweekend.h"

int main() {

  /* Setup image and objects */
  camera cam;
  const int image_width = 200;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  hittable_list world;
  world.add(std::make_shared<sphere>(point3(-1,0,-cam.focal_length), 0.5));
  world.add(std::make_shared<sphere>(point3(1,0,-cam.focal_length), 0.5));
  const int samples_per_pixel = 100;

  /* Setup PPM file */
  std::ofstream img;
  img.open("render_2.ppm");
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  hit_record rec;
  for (int i = 0; i < image_height; ++i) {
    std::cerr << "\rScanlines remaining: " << image_height - i << std::flush;
    for (int j = 0; j < image_width; ++j) {
      double sum = 0;
      for (int k = 0; k < samples_per_pixel; ++k) {
        double u = (j+random_double()) / image_width;
        double v = (i+random_double()) / image_height;
        ray r = cam.get_ray(u, v);
        sum += world.hit(r, -50, 50, rec) ? 1.0 : 0;
      }
     write_color(img, color(sum/samples_per_pixel,0,0)); 
    }
  }

  img.close();
  return 0;
}
