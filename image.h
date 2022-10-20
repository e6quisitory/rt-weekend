#ifndef IMAGE_H
#define IMAGE_H

#include "vec3.h"

typedef pixel* image;
typedef image* image_list;

/* Allocates image on heap and returns pointer to it */
image alloc_image(int width, int height) {
    return new pixel[width * height];        // initialized to black, as per vec3 default constructor
}

/* Allocates an array of images on heap and returns point to array */
image_list alloc_images(int width, int height, int num_images) {
    image_list images = new image[num_images];                    // Allocate an array of image pointers on heap

    for (int i = 0; i < num_images; i++)                          // Fill image pointers array with valid pointers to allocated individual images on heap
        images[i] = alloc_image(width, height);

    return images;
}

/* Takes in a list of images; deletes each image, and then deletes the list itself, out of memory */
void delete_images(image_list images, int num_images) {
    for (int i = 0; i < num_images; ++i)                 // free each image
        delete [] images[i];
    delete [] images;                                    // free list of images
}

/* Takes in list of images, averages them, and returns averaged image */
image average_images(image_list images, int num_images, int width, int height) {
    image averaged = alloc_image(width, height);
    for (int i = 0; i < width * height; ++i) {
        pixel sum;
        for (int j = 0; j < num_images; ++j)
            sum += images[j][i];

        averaged[i] = sum/num_images;
    }
    return averaged;
}

#endif