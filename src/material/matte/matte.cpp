#include "matte.h"

matte::matte(color a) {
  albedo = a;
}

ray matte::scatter(const ray& r_in, const hit_record& rec) const {
  point3 target = rec.p + rec.normal + random_in_unit_sphere();
  return ray(rec.p, target - rec.p);
}