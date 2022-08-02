#ifndef MATTE_H
#define MATTE_H

#include "material.h"
#include "hittable.h"
#include "ray.h"
#include "vec3.h"

class matte : public material {
  public:
    matte(color a) { albedo = a; }
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;
};

ray matte::scatter(const ray& r_in, const hit_record& rec) const {
 point3 target = rec.p + rec.normal + random_in_unit_sphere();
 return ray(rec.p, target - rec.p);
}

#endif
