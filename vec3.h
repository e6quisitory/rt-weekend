#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <cmath>
#include "rtweekend.h"

class vec3 {
  public:
    vec3(double a, double b, double c): e{a,b,c} {}
    vec3(double d): e{d,d,d} {}
    vec3(): e{0,0,0} {} // initialize components to 0 if nothing passed in

    double x() { return e[0]; };
    double y() { return e[1]; };
    double z() { return e[2]; };
    
    double R() { return e[0]; };
    double G() { return e[1]; };
    double B() { return e[2]; };

    /* operators below included as member functions all are concerned with reading/modifying an individual vec3 object. Not concerned with operations *b/w* vec3 objects
       In other words, these are functions you'd call *through* an object. */

    double& operator[](int i) { // if non-const vec3 object, then pass by reference
      return e[i];
    }

    double operator[](int i) const { // if const vec3 object, then pass by value so that vec3 components do not get modified
      return e[i];
    }

    vec3& operator+=(const vec3& v) { // these operators return
      e[0] += v.e[0];
      e[1] += v.e[1];
      e[2] += v.e[2];
      return *this;
    }

    vec3& operator-=(const vec3& v) {
      e[0] -= v.e[0];
      e[1] -= v.e[1];
      e[2] -= v.e[2];
      return *this;
    }

    vec3& operator*=(double t) {
      e[0] *= t;
      e[1] *= t;
      e[2] += t;
      return *this;
    }

    vec3& operator/=(double t) {
      return *this *= 1/t;
    }

    double length() const {
      return std::sqrt(length_squared());
    }

    double length_squared() const {
      return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool isUnitVec() const {
      return (length() == 1) ? true : false;
    }

    inline static vec3 random() {
      return vec3(random_double(), random_double(), random_double());
    }

    inline static vec3 random(double min, double max) {
      return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

  public:
    double e[3];
};

// aliases - same data structure as vec3, just different names
using point3 = vec3; // 3D point
using color = vec3;  // RGB color


// vec3 utility functions, i.e. concerned with operations b/w vec3 objects, and not with modifying/reading an individual object itself.

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
  return out << "[" << v.e[0] << ", " << v.e[1] << ", " << v.e[2] << "]";
}

inline vec3 operator+(const vec3& v1, const vec3& v2) {
  return vec3(v1.e[0]+v2.e[0], v1.e[1]+v2.e[1], v1.e[2]+v2.e[2]);
}

inline vec3 operator-(const vec3& v) {
  return vec3(-v.e[0], -v.e[1], -v.e[2]);
}

inline vec3 operator-(const vec3& v1, const vec3& v2) {
  return v1+(-v2);
}

inline vec3 operator*(const vec3& v, double t) {
  return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
}

inline vec3 operator*(double t, const vec3& v) {
  return vec3(v.e[0]*t, v.e[1]*t, v.e[2]*t);
}

inline vec3 operator*(const vec3& v1, const vec3& v2) {
  return vec3(v1.e[0]*v2.e[0], v1.e[1]*v2.e[1], v1.e[2]*v2.e[2]);
}

inline vec3 operator/(const vec3& v1, double t) {
  return vec3(v1.e[0]/t, v1.e[1]/t, v1.e[2]/t);
}

vec3 operator^(const vec3& v, double d) {
  return vec3( pow(v.e[0], d), pow(v.e[1], d), pow(v.e[2], d) );
}

vec3 sqrt(const vec3& v) {
  return vec3(std::sqrt(v.e[0]), std::sqrt(v.e[1]), std::sqrt(v.e[2]));
}

inline double dot(const vec3& v1, const vec3& v2) {
  return v1.e[0]*v2.e[0] + v1.e[1]*v2.e[1] + v1.e[2]*v2.e[2];
}

inline vec3 cross(const vec3& v1, const vec3& v2) {
  return vec3(v1.e[1] * v2.e[2] - v1.e[2] * v2.e[1],
              v1.e[2] * v2.e[0] - v1.e[0] * v2.e[2],
              v1.e[0] * v2.e[1] - v1.e[1] * v2.e[0]);
}

inline vec3 unit_vector(const vec3& v) {
  return v / v.length();
}

point3 random_in_unit_sphere() {
  while (true) {
    point3 p = vec3::random(-1,1);
    if (p.length_squared() < 1) return p;
  }
}

vec3 reflect(const vec3& v_in, const vec3& n) {
  return v_in - 2*dot(v_in, n)*n;
}

#endif
