#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"

#define MAX_COLOR 255

void write_color(std::ofstream& out, color pixel_color) {
  out << static_cast<int>( 255.999 * pixel_color.R() ) << ' ' 
      << static_cast<int>( 255.999 * pixel_color.G() ) << ' '
      << static_cast<int>( 255.999 * pixel_color.B() ) << '\n';
}
/* Takes in an RGB pixel and returns it as a ARGB8888 Uint32 pixel */
Uint32 convert_to_ARGB8888(color pixel_color) {
  Uint32 full_opacity = 0xFF000000;
  return full_opacity |
         (static_cast<Uint32>(255.999*pixel_color.R()) << 16) |
         (static_cast<Uint32>(255.999*pixel_color.G()) << 8)  |
         (static_cast<Uint32>(255.999*pixel_color.B()));
}

#endif
