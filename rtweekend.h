#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <float.h>
#include <memory>
#include <thread>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <limits>

#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "camera.h"
#include "matte.h"
#include "dielectric.h"
#include "metal.h"
#include "renderer.h"

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

// Timer stuff

typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float>       duration;
typedef std::chrono::minutes               mins;

void print_render_time(duration render_time_duration, std::ostream& out, int desired_precision) {
    mins  render_time_mins       = std::chrono::duration_cast<mins>(render_time_duration);
    float render_time_mins_float = float(render_time_mins.count());
    float render_time_secs_float = render_time_duration.count();
    float render_time_mins_secs  = render_time_mins_float + (render_time_secs_float - 60.0*render_time_mins_float)/60.0;

    int prev_precision = out.precision();
    out.precision(desired_precision);
    out << "\nRender time: " << render_time_secs_float << "s, " << render_time_mins_secs << " mins"<<std::endl;
    out.precision(prev_precision);
}

#endif
