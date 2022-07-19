#include <iostream>
#include <fstream>
#include <float.h>
#include <memory>
#include "rtweekend.h"

int main() {

  /* Setup image and objects */
  camera cam;
  const int image_width = 400;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  hittable_list world;
  world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
  world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
  const int samples_per_pixel = 10;

  /* Setup PPM file */
  std::ofstream img;
  img.open("render.ppm");
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  hit_record rec;
  for (int i = image_height; i > 0; --i) { // must go from top to bottom as PPM file starts in top left corner, not bottom
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
