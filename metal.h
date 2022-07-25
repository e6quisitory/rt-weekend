#ifndef METAL_H
#define METAL_H

#include "material.h"
#include "hittable.h"
#include "vec3.h"
#include "ray.h"

class metal : public material {
  public:
    metal(color a): albedo(a) {}
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;
    virtual color get_albedo() const override;

  public:
    color albedo;
};

ray metal::scatter(const ray& r_in, const hit_record& rec) const {
  vec3 idirection = r_in.direction();
  vec3 normal = rec.normal;
  return ray(rec.p, idirection - 2*(normal*dot(idirection, normal)));
}

color metal::get_albedo() const {
  return albedo;
}

#endif
