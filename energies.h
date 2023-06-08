#ifndef ENERGIES
#define ENERGIES

#include "def.h"

float euclidian_color_dist(Color col1, Color col2) {
    return sqrt( pow(col1.r - col2.r, 2) + pow(col1.g - col2.g, 2) + pow(col1.b - col2.b, 2));
}

float euclidian_energy(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth) {
    float c = 0.;
    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            if (i < numPixelsHeight - 1) {
                c += euclidian_color_dist(pixels[idx(i, j, numPixelsWidth)].color, pixels[idx(i+1, j, numPixelsWidth)].color);
                if (j < numPixelsWidth - 1) {
                    c += euclidian_color_dist(pixels[idx(i, j, numPixelsWidth)].color, pixels[idx(i+1, j+1, numPixelsWidth)].color);
                }
            }
            if (j < numPixelsWidth - 1) {
                c += euclidian_color_dist(pixels[idx(i, j, numPixelsWidth)].color, pixels[idx(i, j+1, numPixelsWidth)].color);
            }
        }
    }

    return c;
}

float vertical_straight_energy(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth) {
    float c = 0.;
    
    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            c += euclidian_color_dist(pixels[idx(i, j, numPixelsWidth)].color, (Color){ 0,0,0,255}) * (pow((float)j - (float)numPixelsWidth / 2., 4));
        }
    }
    return c;
}


#endif