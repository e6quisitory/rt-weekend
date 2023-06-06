#pragma once

#include "../hittable.h"

class hittable_list : public hittable {
  public:
    hittable_list();
    hittable_list(hittable::ptr h_object);

    void clear();
    void add(hittable::ptr h_object);
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& h) const override;

  public:
    hittable::ptr_list h_list;
};