#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "ray.h"
#include "rtweekend.h"

class material {
  public:
    virtual ray scatter(const ray& r_in, const hit_record& rec) const = 0;
    virtual color get_albedo() const = 0;
};

#endif
