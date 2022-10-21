#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.h"

class image {
public:
    image(const int w, const int h): width(w), height(h) {
        pixels = new pixel*[width];
        for (int i = 0; i < width; ++i)
            pixels[i] = new pixel[height];
    }

    ~image() {
        for (int i = 0; i < width; ++i)
            delete[] pixels[i];
        delete[] pixels;
        std::cout << "image deleted out of memory" << std::endl;
    }

    pixel& operator [] (int index) {   // If operator is called on non-const object, allow modification, hence return by reference.
        return pixels[index % width][index / width];
    }

    pixel operator [] (int index) const {   // If operator is called on const object, do not allow modification, hence return by value.
        return pixels[index % width][index / width];
    }

    pixel& operator () (int x, int y) {
        return pixels[x][y];
    }

    pixel operator () (int x, int y) const {
        return pixels[x][y];
    }

public:
    int width;
    int height;
    pixel** pixels;
};

#endif