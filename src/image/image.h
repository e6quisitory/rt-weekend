#pragma once

class image {
  public:
    image(int w, int h);

    ~image();

    Uint32& operator [] (int index);
    Uint32  operator [] (int index)     const;
    Uint32& operator () (int x, int y);
    Uint32  operator () (int x, int y)  const;
        
  public:
    Uint32* pixels;
    int width;
    int height;
};