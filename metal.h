#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "hittable.h"
#include "vec3.h"
#include "ray.h"

class metal : public material {
  public:
    metal(color a, double f): albedo(a), fuzz(f <= 1 ? f : 1) {}
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;
    virtual color get_albedo() const override;

  public:
    color albedo;
    double fuzz;
};

ray metal::scatter(const ray& r_in, const hit_record& rec) const {
  vec3 reflected = reflect(r_in.direction(), rec.normal);
  return ray(rec.p, reflected + fuzz*random_in_unit_sphere());
}

color metal::get_albedo() const {
  return albedo;
}

#endif
