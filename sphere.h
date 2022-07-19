#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include <cmath>

class sphere : public hittable {
  public:
    sphere() {}
    sphere(point3 c, double r): center(c), radius(r) {}

    virtual bool hit(const ray &r, double t_min, double t_max, hit_record &rec) const override;

  public:
    point3 center;
    double radius;
};

bool sphere::hit(const ray& r, double t_min, double t_max, hit_record& rec) const {
  // Hard-code quadratic solution for ray-sphere intersection
  vec3 oc = r.origin()-center;
  double a = dot(r.direction(), r.direction());
  double b = 2*dot(oc, r.direction()); // b will always be < 0 as the angle b/w dotted vectors always > 90 deg 
  double c = dot(oc, oc) - radius*radius; 
  double discriminant = b*b - 4*a*c; 

  if (discriminant < 0) // no real solution
    return false;
  else {                // one or two solutions
    double root = (-b - std::sqrt(discriminant))/2.0*a;  

    if (root < t_min || root > t_max) {
      root = (-b + std::sqrt(discriminant))/2.0*a;
      if (root < t_min || root > t_max)
        return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    vec3 outward_normal = (rec.p - center)/radius;
    rec.set_face_normal(r, outward_normal);
    return true;
  }
}

#endif
