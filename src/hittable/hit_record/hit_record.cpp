#include "hit_record.h"

void hit_record::set_face_normal(const ray &r, const vec3 &outward_normal) {
  is_front_face = dot(r.direction(), outward_normal) < 0;
  normal = is_front_face ? outward_normal : -outward_normal;
}