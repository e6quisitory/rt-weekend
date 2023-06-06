#include "color.h"

Uint32 convert_to_ARGB8888(const color& pixel_color) {
  Uint32 full_opacity = 0xFF000000;
  return full_opacity |
    (static_cast<Uint32>(255.999*pixel_color.R()) << 16) |
    (static_cast<Uint32>(255.999*pixel_color.G()) << 8)  |
    (static_cast<Uint32>(255.999*pixel_color.B()));
}

void write_ARGB8888_PPM(std::ofstream& out, const Uint32& ARGB8888) {
  int r = (ARGB8888 & 0x00FF0000) >> 16;
  int g = (ARGB8888 & 0x0000FF00) >> 8;
  int b = (ARGB8888 & 0x000000FF);
  out << r << ' ' << g << ' ' << b << '\n';
}