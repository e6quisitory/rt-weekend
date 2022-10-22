#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include <memory>
#include "material.h"
#include "vec3.h"
#include "ray.h"

class dielectric : public material {
  public:
    typedef std::shared_ptr<dielectric> ptr;

    dielectric(double n) {
      albedo = color(0.98);
      refractive_index = n;
    }
    virtual ray scatter(const ray& r_in, const hit_record& rec) const override;  

  public:
    double refractive_index;

  private:
    static double reflectance(double n1, double n2, double theta1) {
      double r0 = pow(((n1-n2)/(n1+n2)), 2);
      return r0 + (1.0-r0)*pow((1-std::cos(theta1)), 5);
    }
};

// optimization needed of this function
ray dielectric::scatter(const ray& r_in, const hit_record& rec) const {
  double n1 = rec.is_front_face ? 1.0 : refractive_index;
  double n2 = rec.is_front_face ? refractive_index : 1.0;

   double theta1 = angle_bw(-r_in.direction(), rec.normal);
   double theta2 = std::asin((n1/n2)*std::sin(theta1));

   if (rec.is_front_face && reflectance(n1, n2, theta1) > 0.3) // play around with this number
    return ray(rec.p, reflect(r_in.direction(), rec.normal));

    vec3 tangent = unit_vector(r_in.direction() + r_in.direction().length()*std::cos(theta1)*rec.normal);

    bool tir = false;
    if (theta2 > 1.570795) {  // total internal reflection
      theta2 = theta2 - 1.570795;
      tir = true;
    }   

    vec3 t_perp = std::cos(theta2)*rec.normal;
    vec3 t_par = std::sin(theta2)*tangent;

    return tir ? ray(rec.p, t_perp + t_par) : ray(rec.p, -t_perp + t_par);
   
}


#endif
