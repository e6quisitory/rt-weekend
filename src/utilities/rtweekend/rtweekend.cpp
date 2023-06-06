#include "rtweekend.h"

double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

double random_double() {
    // Returns a random real in [0,1).
    return rand() / (RAND_MAX + 1.0);
}

double random_double(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*random_double();
}

void print_render_time(duration render_time_duration, std::ostream& out, int desired_precision) {
    mins  render_time_mins       = std::chrono::duration_cast<mins>(render_time_duration);
    float render_time_mins_float = float(render_time_mins.count());
    float render_time_secs_float = render_time_duration.count();
    float render_time_mins_secs  = render_time_mins_float + (render_time_secs_float - 60.0*render_time_mins_float)/60.0;

    int prev_precision = out.precision();
    out.precision(desired_precision);
    out << "\nRender complete.\nRender time: " << render_time_secs_float << "s, " << render_time_mins_secs << " mins"<<std::endl;
    out.precision(prev_precision);
}