#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "vec3.h"

class camera {
  public:
    camera(point3 lookfrom, point3 lookat, vec3 vup, double aspr, double y_fov) {
      /* Set public vars */
      aspect_ratio = aspr;
      focal_length = 1.0;
      
      /* Use vertical FOV to set viewport dimensions */
      double h = std::tan(y_fov/2.0)*focal_length;
      double viewport_height = 2.0*h;
      double viewport_width = aspect_ratio * viewport_height;

      /* Camera basis vectors */
      vec3 view_dir = unit_vector(lookat - lookfrom);
      vec3 x = unit_vector(cross(view_dir, vup));
      vec3 y = cross(-x, -view_dir);

      /* Set image plane vars */
      origin = lookfrom;
      horizontal = x * viewport_width;
      vertical = y * viewport_height;
      lower_left_corner = origin + focal_length * view_dir - horizontal/2 - vertical/2;
    }

    // u, v are real numbers b/w 0 and 1. Width and height of the viewport represented as a percentage.
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
