#pragma once

#include "../material.h"
#include "../../hittable/hit_record/hit_record.h"

class dielectric : public material {
  public:
    typedef std::shared_ptr<dielectric> ptr;

    dielectric(double n);
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;  

  public:
    double refractive_index;

  private:
    static double reflectance(double n1, double n2, double theta1);
};