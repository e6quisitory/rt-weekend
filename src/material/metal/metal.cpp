#include "metal.h"

metal::metal(color a, double f) {
  albedo = a;
  fuzz = f <= 1 ? f : 1;
}

ray metal::scatter(const ray& r_in, const hit_record& rec) const {
  vec3 reflected = reflect(r_in.direction(), rec.normal);
  return ray(rec.p, reflected + fuzz*random_in_unit_sphere());
}