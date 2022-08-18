#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class ray {
  public:
    ray(const point3& origin, const vec3& direction):
      orig(origin),
      dir(direction)
      {}
    
    ray() {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const {   // returns point t units down the ray from the origin
      return orig + t*dir;
    }

  public:
    // all you need to define a ray is an origin and a direction
    point3 orig;
    vec3 dir; // unit vector
};

#endif
