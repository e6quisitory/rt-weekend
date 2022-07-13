#include <iostream>
#include <fstream>
#include <float.h>
#include "rtweekend.h"

int main() {

  /* Setup image and objects */
  camera cam;
  const int image_width = 800;
  const int image_height = image_width / cam.aspect_ratio; // image & viewport have same aspect ratio
  sphere ball = sphere(point3(0,0,-cam.focal_length), 0.5); 
  const int samples_per_pixel = 100;

  /* Setup PPM file */
  std::ofstream img;
  img.open("render.ppm");
  img << "P3\n" << image_width << ' ' << image_height << "\n255\n"; 

  /* Render image */
  for (int i = 0; i < image_height; ++i) {
    std::cerr << "\rScanlines remaining: " << image_height - i << std::flush;
    for (int j = 0; j < image_width; ++j) {
      double sum = 0;
      for (int k = 0; k < samples_per_pixel; ++k) {
        double u = (j+random_double()) / image_width;
        double v = (i+random_double()) / image_height;
        ray r = cam.get_ray(u, v);
        sum += ball.hit(r, -50, 50) ? 1.0 : 0;
      }
     write_color(img, color(sum/samples_per_pixel,0,0)); 
    }
  }

  img.close();
  return 0;
}
