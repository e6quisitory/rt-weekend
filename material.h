#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "vec3.h"

struct hit_record;

class material {
  public:
    typedef std::shared_ptr<material> ptr;

    virtual ray scatter(const ray& r_in, const hit_record& rec) const = 0;  

  public:
    color albedo;
};

#endif
