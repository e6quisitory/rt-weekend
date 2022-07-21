#ifndef COLOR_H
#define COLOR_H

#define MAX_COLOR 255

#include <iostream>
#include <cmath>
#include "vec3.h"

void write_color(std::ofstream& out, color pixel_color) {
  out << static_cast<int>( 255.999 * pixel_color.R() ) << ' ' 
      << static_cast<int>( 255.999 * pixel_color.G() ) << ' '
      << static_cast<int>( 255.999 * pixel_color.B() ) << '\n';
}

#endif
