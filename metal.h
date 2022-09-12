#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "rtweekend.h"

class metal : public material {
  public:
    typedef std::shared_ptr<metal> ptr;
    
    metal(color a, double f) {
      albedo = a;
      fuzz = f <= 1 ? f : 1;
    }
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;

  public:
    double fuzz;
};

ray metal::scatter(const ray& r_in, const hit_record& rec) const {
  vec3 reflected = reflect(r_in.direction(), rec.normal);
  return ray(rec.p, reflected + fuzz*random_in_unit_sphere());
}

#endif
