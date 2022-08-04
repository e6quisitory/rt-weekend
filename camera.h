#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
  public:
    camera(double aspr, double y_fov) {
      aspect_ratio = aspr;
      focal_length = 1.0;
      
      double h = std::tan(degrees_to_radians(y_fov/2.0))*focal_length;
      double viewport_height = 2.0*h;
      double viewport_width = aspect_ratio * viewport_height;

      origin = point3(0,0,0);
      horizontal = vec3(viewport_width, 0, 0);
      vertical = vec3(0, viewport_height, 0);
      lower_left_corner = origin - horizontal/2 - vertical/2 - vec3(0, 0, focal_length);
    }

    // u, v are real numbers b/w 0 and 1. Basically the width and height of the viewport represented as a percentage.
    ray get_ray(double u, double v) const {
      return ray(origin, (lower_left_corner + u*horizontal + v*vertical) - origin);
    }

  public:
    double aspect_ratio;
    double focal_length;

  private:
    point3 origin;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;
};

#endif
