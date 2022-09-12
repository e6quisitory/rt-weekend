#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <vector>
#include "hittable.h"
#include "ray.h"

class hittable_list : public hittable {
  public:
    hittable_list() {}
    hittable_list(hittable::ptr h_object) { add(h_object); };

    void clear() { h_list.clear(); }
    void add(hittable::ptr h_object) { h_list.push_back(h_object); }
    
    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& h) const override;

  public:
    hittable::ptr_list h_list;
};

bool hittable_list::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  hit_record temp_rec;
  double t_closest = t_max;
  bool hit_anything = false;
  for (const hittable::ptr h_object : h_list) {
    if (h_object->hit(r, t_min, t_closest, temp_rec)) {           
      hit_anything = true;
      t_closest = temp_rec.t;
      rec = temp_rec;
    }
  }
  return hit_anything;
}

#endif
