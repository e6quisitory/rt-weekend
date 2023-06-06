#pragma once

#include "../material.h"
#include "../../hittable/hittable.h"
#include "../../hittable/hit_record/hit_record.h"

class metal : public material {
  public:
    typedef std::shared_ptr<metal> ptr;
    
    metal(color a, double f);

    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;

  public:
    double fuzz;
};