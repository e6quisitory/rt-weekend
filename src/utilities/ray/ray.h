#pragma once

#include "../vec3/vec3.h"

class ray {
  public:
    ray(const point3& origin, const vec3& direction);
    ray();

    point3 origin() const;
    vec3 direction() const;

    point3 at(double t) const;   // returns point t units down the ray from the origin

  public:
    point3 orig;
    vec3 dir; // unit vector
};