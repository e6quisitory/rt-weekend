#include <stdio.h>
#include <cmath>
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "sphere.h"
#include <float.h>

int main() {
  // Setup image and objects
  const int image_width = 960;
  const int image_height = 540;

  const point3 origin  = point3(0,0,0);
  const double screen_dist = 0.5*image_width;
  const double sphere_radius = 0.25*image_width; 
  const double sphere_screen_dist = sphere_radius;
  sphere ball = sphere(point3(0,0, screen_dist + sphere_screen_dist), sphere_radius); 

  // Setup ppm file
  std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
  color red = color(1,0,0);
  color black = color(0,0,0);

  for (int i = 0; i < image_height; i++) {
    //std::cerr << "\rScanlines remaining: " << image_height - i << std::flush;
    for (int j = 0; j < image_width; j++) {

      // Calculate ray from eye to pixel
      ray eyeToPixel = ray(origin, vec3(j-image_width/2, i-image_height/2, screen_dist));
      
      // Write pixel color depending on sphere intersection
      hit_record ball_hit;
      if (ball.hit(eyeToPixel, -DBL_MAX, DBL_MAX, ball_hit))
        write_color(std::cout, color(0,0,1));
      else
        write_color(std::cout, color(0,0,0));
    }
  }

  return 0;
}
