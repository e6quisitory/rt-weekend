#pragma once

#include "../../material/material.h"

struct hit_record {
  point3 p;
  vec3 normal;
  double t;
  material::ptr material_ptr;
  bool is_front_face;

  void set_face_normal(const ray &r, const vec3 &outward_normal);
};