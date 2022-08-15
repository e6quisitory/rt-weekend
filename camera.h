#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "vec3.h"

class camera {
  public:
    camera(point3 lookfrom, point3 lookat, point3 focusat, vec3 vup, double aspr, double y_fov, double aperature) {

      /* Set public vars */
      aspect_ratio = aspr;

      vec3 looking_dir = lookat - lookfrom;
      vec3 focus_dir = focusat - lookfrom;
      focus_dist = focus_dir.length()*std::cos(angle_bw(looking_dir, focus_dir)); // project focus_dir onto looking_dir

      /* Camera basis vectors */
      view_dir = unit_vector(lookat - lookfrom);
      x = unit_vector(cross(view_dir, vup));
      y = cross(-x, -view_dir);

      /* Use vertical FOV to set viewport dimensions */
      double h = std::tan(degrees_to_radians(y_fov)/2.0)*focus_dist;
      double viewport_height = 2.0*h;
      double viewport_width = aspect_ratio * viewport_height;

      /* Set image/focus plane vars & lens radius*/
      origin = lookfrom;
      horizontal = x * viewport_width;
      vertical = y * viewport_height;
      lower_left_corner = origin + focus_dist * view_dir - horizontal/2 - vertical/2;
      lens_radius = aperature/2;
    
    }

    vec3 random_in_unit_disk() const {
      while (true) {
        vec3 v = random_double()*x + random_double()*y;
        if (v.length_squared() < 1) return v;
      }
    }

    // u, v are real numbers b/w 0 and 1. Width and height of the viewport represented as a percentage.
    ray get_ray(double u, double v) const {
      vec3 rd = origin + lens_radius*random_in_unit_disk();
      return ray(rd, (lower_left_corner + u*horizontal + v*vertical) - rd);
    }

  public:
    double aspect_ratio;
    double focus_dist;

  private:
    point3 origin;
    vec3 view_dir;
    vec3 x;
    vec3 y;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;
    double lens_radius;
};

#endif
