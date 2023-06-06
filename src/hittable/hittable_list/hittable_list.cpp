#include "hittable_list.h"

hittable_list::hittable_list() {}

hittable_list::hittable_list(hittable::ptr h_object) {
  add(h_object);
};

void hittable_list::clear() {
  h_list.clear();
}

void hittable_list::add(hittable::ptr h_object) {
  h_list.push_back(h_object);
}

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