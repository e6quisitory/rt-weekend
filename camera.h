#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

class camera {
  public:
    camera(point3 eye, double x_ang, double y_ang, double aspr, double y_fov) {
      x_ang = degrees_to_radians(x_ang);
      y_ang = degrees_to_radians(y_ang);
      y_fov = degrees_to_radians(y_fov);

      aspect_ratio = aspr;
      focal_length = 1.0;
      
      double h = std::tan(y_fov/2.0)*focal_length;
      double viewport_height = 2.0*h;
      double viewport_width = aspect_ratio * viewport_height;

      vec3 flat = -z_hat()*std::cos(x_ang) + x_hat()*std::sin(x_ang);
      vec3 view_dir = flat*std::cos(y_ang) + y_hat()*std::sin(y_ang);

      vertical = viewport_height*(y_hat()*std::cos(y_ang) - flat*std::sin(y_ang));
      horizontal = viewport_width*(x_hat()*std::cos(x_ang) + z_hat()*std::sin(x_ang));

      origin = eye;
      lower_left_corner = origin + focal_length*view_dir - horizontal/2 - vertical/2;
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
