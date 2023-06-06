#include "image.h"

image::image(int w, int h): width(w), height(h) {
  pixels = new Uint32[w*h];
  for (int i = 0; i < w*h; ++i)
    pixels[i] = 0x00000000;
}

image::~image() {
  delete[] pixels;
}

Uint32& image::operator [] (int index) {
  return pixels[index];
}

Uint32 image::operator [] (int index) const {
  return pixels[index];
}

Uint32& image::operator () (int x, int y) {
  return pixels[y*width + x];
}

Uint32 image::operator () (int x, int y) const {
  return pixels[y*width + x];
}