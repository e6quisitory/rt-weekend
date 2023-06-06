#pragma once

#include "../rtweekend/rtweekend.h"

class vec3 {
  public:
    vec3(double a, double b, double c);
    vec3(double d);
    vec3();

    double x() const;
    double y() const;
    double z() const;
    
    double R() const;
    double G() const;
    double B() const;

    /* operators below included as member functions all are concerned with reading/modifying an individual vec3 object. Not concerned with operations *b/w* vec3 objects
       In other words, these are functions you'd call *through* an object. */

    double& operator [] (int i);
    double  operator [] (int i) const;
    vec3&   operator += (const vec3& v);
    vec3&   operator -= (const vec3& v);
    vec3&   operator *= (double t);
    vec3&   operator /= (double t);
    double  length()         const;
    double  length_squared() const;
    bool    isUnitVec()      const;

    inline static vec3 random();
    inline static vec3 random(double min, double max);

  public:
    double e[3];
};

// aliases - same data structure as vec3, just different names
using point3 = vec3; // 3D point
using color = vec3;  // RGB color
using pixel = vec3;

// vec3 utility functions, i.e. concerned with operations b/w vec3 objects, and not with modifying/reading an individual object itself.
inline std::ostream& operator<<(std::ostream& out, const vec3& v);

vec3 operator+ (const vec3& v1, const vec3& v2);
vec3 operator- (const vec3& v);
vec3 operator- (const vec3& v1, const vec3& v2);
vec3 operator* (const vec3& v, double t);
vec3 operator* (double t, const vec3& v);
vec3 operator* (const vec3& v1, const vec3& v2);
vec3 operator/ (const vec3& v1, double t);
vec3 operator/ (const vec3& v1, int t);
vec3 operator^ (const vec3& v, double d);

vec3 sqrt(const vec3& v);
double dot(const vec3& v1, const vec3& v2);
vec3 cross(const vec3& v1, const vec3& v2);
vec3 unit_vector(const vec3& v);

point3 random_in_unit_sphere();
vec3 reflect(const vec3& v_in, const vec3& n);

double angle_bw(const vec3& v1, const vec3& v2);

vec3 x_hat();
vec3 y_hat();
vec3 z_hat();