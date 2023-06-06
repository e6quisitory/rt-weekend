#pragma once

#include "hit_record/hit_record.h"
#include "../material/material.h"

class hittable {
  public:
    typedef std::shared_ptr<hittable> ptr;
    typedef std::vector<ptr> ptr_list;
    
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const = 0;
};