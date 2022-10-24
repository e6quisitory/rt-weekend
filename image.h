#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.h"
#include <SDL2/SDL.h>

class image {
  public:
    image(int w, int h): width(w), height(h) {
      pixels = new Uint32[w*h];
        for (int i = 0; i < w*h; ++i)
          pixels[i] = 0x00000000;
      }

    ~image() {
      delete[] pixels;
    }

    Uint32& operator [] (int index) {   // If operator is called on non-const object, allow modification, hence return by reference.
      return pixels[index];
    }

    Uint32 operator [] (int index) const {   // If operator is called on const object, do not allow modification, hence return by value.
      return pixels[index];
    }

    Uint32& operator () (int x, int y) {
      return pixels[y*width + x];
    }

    Uint32 operator () (int x, int y) const {
      return pixels[y*width + x];
    }
        
    public:
      Uint32* pixels;
      int width;
      int height;
};

#endif