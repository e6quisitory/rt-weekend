#include "sphere.h"

sphere::sphere() {}

sphere::sphere(point3 c, double r, material::ptr m): center(c), radius(r), material_ptr(m) {}

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  vec3 ray_direction = r.direction();
  vec3 origin_centre = r.origin() - center;
  double a = ray_direction.length_squared();
  double b = 2*dot(ray_direction, origin_centre);
  double c = origin_centre.length_squared() - radius*radius;

  double discriminant = b*b - 4*a*c;

  if (discriminant < 0) return false;

  // check which root is within range
  double t = (-b - std::sqrt(discriminant))/(2*a);
  if (t < t_min || t > t_max) {               // if first root is out of range, check second root
    t = (-b + std::sqrt(discriminant))/(2*a);
    if (t < t_min || t > t_max) return false;
  }
  // root in range has been found
  rec.t = t;
  rec.p = r.at(rec.t);
  vec3 outward_normal = (rec.p - center)/radius;
  rec.set_face_normal(r, outward_normal);
  rec.material_ptr = material_ptr;

  return true;
}