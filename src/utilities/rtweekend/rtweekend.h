#pragma once

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
double degrees_to_radians(double degrees);
double random_double();
double random_double(double min, double max);

// Timer stuff
typedef std::chrono::high_resolution_clock Time;
typedef std::chrono::duration<float>       duration;
typedef std::chrono::minutes               mins;
typedef std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<float>> time_val;

void print_render_time(duration render_time_duration, std::ostream& out, int desired_precision);