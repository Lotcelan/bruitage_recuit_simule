#include <stdio.h>
#include <time.h>
#include "utils.h"

#define COMPLETE_RANDOM 0
#define GAUSSIAN 1

/*
#define RAYGUI_IMPLEMENTATION
#include "./raygui.h"

#undef RAYGUI_IMPLEMENTATION     
*/


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


void runMainLoop(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth, float pixelSize, int mode) {
    //BeginDrawing();
    float temperature = 150;
    float decayFactor = 0.999;
    float finalTemperature = 0;

    int iteration = 0;
    int freqDisplay = 700;

    CustomPixel* neighbourPixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);

    float delta = 0;
    float p = 0;

    while (temperature > finalTemperature) {
        iteration++;
        copyArrayTo(neighbourPixels, pixels, numPixelsHeight, numPixelsWidth);
        //swapRandomPixels(neighbourPixels, numPixelsHeight, numPixelsWidth);
        swapRandomPixelsColors(neighbourPixels, numPixelsHeight, numPixelsWidth);
        

        switch (mode)
        {
        case 0:
            delta = 0;
            break;
        case 1:
            delta = euclidian_energy(pixels, numPixelsHeight, numPixelsWidth) -  euclidian_energy(neighbourPixels, numPixelsHeight, numPixelsWidth);
            break;
        default:
            delta = 0;
            break;
        }
        

        // On veut minimiser l'énergie
        if (delta >= 0) {
            copyArrayTo(pixels, neighbourPixels, numPixelsHeight, numPixelsWidth);
        } else {
            p = randomFloat();
            if (exp(delta / (temperature * sqrt(numPixelsHeight * numPixelsWidth))) >= p) {
                copyArrayTo(pixels, neighbourPixels, numPixelsHeight, numPixelsWidth);
            }
        }
        
        if (iteration == freqDisplay) {
            iteration = 0;
            //BeginDrawing();
            draw_all_pixels(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
            //EndDrawing();
        }
        temperature *= decayFactor;
    }
    //EndDrawing();
}

int main(void) {
    srand(time(NULL));

    const int screenWidth = 800;
    const int screenHeight = 600;

    // Initialisation des paramètres
    
    float pixelSize = 80.0f;

    int mode = 1;


    InitWindow(screenWidth, screenHeight, "Exemple de fenêtre avec paramètres");

    int numPixelsWidth = screenWidth / pixelSize;
    int numPixelsHeight = screenHeight / pixelSize;

    CustomPixel* pixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);

    // Initialisation des pixels personnalisés avec des couleurs aléatoires
    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            pixels[idx(i,j,numPixelsWidth)].i = i;
            pixels[idx(i,j,numPixelsWidth)].j = j;
            int random = GetRandomValue(0, 255);
            pixels[idx(i,j,numPixelsWidth)].color = (Color){random, random, random, 255};
        }
    }

    runMainLoop(pixels, numPixelsHeight, numPixelsWidth, pixelSize, mode);


    CloseWindow();

    return 0;
}
