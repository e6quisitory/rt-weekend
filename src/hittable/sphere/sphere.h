#pragma once

#include "../hittable.h"
#include "../../material/material.h"

class sphere : public hittable {
  public:
    sphere();
    sphere(point3 c, double r, material::ptr m);

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

  public:
    point3 center;
    double radius;
    material::ptr material_ptr;
};