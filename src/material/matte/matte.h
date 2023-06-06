#pragma once

#include "../material.h"
#include "../../hittable/hittable.h"
#include "../../hittable/hit_record/hit_record.h"

class matte : public material {
  public:
    typedef std::shared_ptr<matte> ptr;

    matte(color a);
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;
};