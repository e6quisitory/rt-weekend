#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include "vec3.h"
#include "ray.h"

class material;

struct hit_record {
  point3 p;
  vec3 normal;
  double t;
  std::shared_ptr<material> material_ptr;
  bool is_front_face;

  inline void set_face_normal(const ray &r, const vec3 &outward_normal) {
    is_front_face = dot(r.direction(), outward_normal) < 0;
    normal = is_front_face ? outward_normal : -outward_normal;
  }
};

class hittable {
  public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};

#endif
