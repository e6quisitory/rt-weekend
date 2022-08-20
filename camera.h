#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"
#include "vec3.h"
#include "ray.h"

class camera {
  public:
    camera() {}
    camera(point3 lookfrom, point3 lookat, point3 focusat, vec3 vup, double aspr, double yfov, double aperature) {
      aspect_ratio = aspr;
      y_fov = yfov;
      lens_radius = aperature/2;

      orient(lookfrom, lookat, vup);
      focus(focusat);
    }

    void orient(point3 lookfrom, point3 lookat, vec3 vup);
    void focus(point3 focusat);
    ray get_ray(double u, double v) const;

  private:
    void set_basis(point3 lookfrom, point3 lookat, vec3 vup);
    void set_focus_distance(point3 focusat);
    void set_viewport_specs();
    void set_imageplane_vecs();
    vec3 random_in_unit_disk() const;

  public:
    double aspect_ratio;
    point3 origin;

  private:
    vec3 view_dir;
    vec3 x;
    vec3 y;
    vec3 horizontal;
    vec3 vertical;
    point3 lower_left_corner;
    double lens_radius;
    double y_fov;
    double focus_dist;
    double viewport_height;
    double viewport_width;
};

void camera::orient(point3 lookfrom, point3 lookat, vec3 vup) {
  /* Camera basis vectors */
  set_basis(lookfrom, lookat, vup);

  // Focus distance and viewport specs remain the same

  /* Set image/focus plane vars */
  set_imageplane_vecs();
}

/* Focus is solely determined by focus distance (assuming aspect ratio remains constant) */
void camera::focus(point3 focusat) {
  /* Set focus distance */
  set_focus_distance(focusat);

  /* Use vertical FOV to set viewport dimensions */
  set_viewport_specs();

  /* Set image/focus plane vars */
  set_imageplane_vecs();
}

void camera::set_basis(point3 lookfrom, point3 lookat, vec3 vup) {
  origin = lookfrom;
  view_dir = unit_vector(lookat - origin);
  x = unit_vector(cross(view_dir, vup));
  y = cross(-x, -view_dir);
}

void camera::set_focus_distance(point3 focusat) {
  vec3 focus_dir = focusat - origin;
  focus_dist = focus_dir.length()*std::cos(angle_bw(view_dir, focus_dir)); // project focus_dir onto looking_dir
}

void camera::set_viewport_specs() {
  double h = std::tan(degrees_to_radians(y_fov)/2.0)*focus_dist;
  viewport_height = 2.0*h;
  viewport_width = aspect_ratio * viewport_height;
}

void camera::set_imageplane_vecs() {
  horizontal = x * viewport_width;
  vertical = y * viewport_height;
  lower_left_corner = origin + focus_dist * view_dir - horizontal/2 - vertical/2;
}

vec3 camera::random_in_unit_disk() const {
  while (true) {
    vec3 v = random_double()*x + random_double()*y;
    if (v.length_squared() < 1) return v;
  }
}

/* u, v are real numbers b/w 0 and 1. Width and height of the viewport represented as a percentage. */
ray camera::get_ray(double u, double v) const {
  vec3 rd = origin + lens_radius*random_in_unit_disk();
  return ray(rd, (lower_left_corner + u*horizontal + v*vertical) - rd);
}

#endif
