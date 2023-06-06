#pragma once

#include "../vec3/vec3.h"

#define MAX_COLOR 255

/* Takes in an RGB pixel and returns it as a ARGB8888 Uint32 pixel */
Uint32 convert_to_ARGB8888(const color& pixel_color);

/* Writes ARGB8888 pixel into PPM output file stream */
void write_ARGB8888_PPM(std::ofstream& out, const Uint32& ARGB8888);