#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "energies.h"

#define COMPLETE_RANDOM 0
#define EUCLIDIAN 1
#define STRAIGHT_VERTICAL 2

/*
#define RAYGUI_IMPLEMENTATION
#include "./raygui.h"

#undef RAYGUI_IMPLEMENTATION     
*/




void runMainLoop(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth, float pixelSize, int mode) {
    //BeginDrawing();
    float temperature = 150;
    float decayFactor = 0.999;
    float finalTemperature = 0;

    int iteration = 0;
    int freqDisplay = 1;

    CustomPixel* neighbourPixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);
    copyArrayTo(neighbourPixels, pixels, numPixelsHeight, numPixelsWidth);

    float delta = 0;
    float p = 0;

    Couple lastModified = (Couple){ .p1 = (Point){ .i = 0, .j = 0}, .p2 = (Point){ .i = 0, .j = 0} };
    bool shouldModify = false;

    while (temperature > finalTemperature) {
        iteration++;

        if (shouldModify) {
            changeCouplePixelInArray(pixels, neighbourPixels, lastModified, numPixelsWidth);
        } else {
            changeCouplePixelInArray(neighbourPixels, pixels, lastModified, numPixelsWidth);

        }

        lastModified = swapRandomPixels(neighbourPixels, numPixelsHeight, numPixelsWidth);
        //lastModified = swapRandomPixelsColors(neighbourPixels, numPixelsHeight, numPixelsWidth);
        

        switch (mode)
        {
        case 0:
            delta = 0;
            break;
        case 1:
            delta = euclidian_energy(pixels, numPixelsHeight, numPixelsWidth) -  euclidian_energy(neighbourPixels, numPixelsHeight, numPixelsWidth);
            break;
        case 2:
            delta = vertical_straight_energy(pixels, numPixelsHeight, numPixelsWidth) - vertical_straight_energy(neighbourPixels, numPixelsHeight, numPixelsWidth);
            break;
        default:
            delta = 0;
            break;
        }
        

        // On veut minimiser l'énergie
        if (delta >= 0) {
            shouldModify = true;
        } else {
            p = randomFloat();
            if (exp(delta / (temperature * sqrt(numPixelsHeight * numPixelsWidth))) >= p) {
                shouldModify = true;    
            }
            else {
                shouldModify = false;
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

    const int screenWidth = 256;
    const int screenHeight = 256;

    // Initialisation des paramètres
    
    float pixelSize = 16.0;

    int mode = STRAIGHT_VERTICAL;


    InitWindow(screenWidth, screenHeight, "Exemple de fenêtre avec paramètres");

    int numPixelsWidth = screenWidth / pixelSize;
    int numPixelsHeight = screenHeight / pixelSize;



    CustomPixel* pixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);

    Image image = LoadImage("images/logo16.png");  // Load image data into CPU memory (RAM)

    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            pixels[idx(i,j,numPixelsWidth)].i = i;
            pixels[idx(i,j,numPixelsWidth)].j = j;
            // Initialisation des pixels personnalisés avec des couleurs aléatoires
            
            //int random = GetRandomValue(0, 255);
            //pixels[idx(i,j,numPixelsWidth)].color = (Color){random, random, random, 255};
            

            // Chargment image
            //printf("Color : %i %i %i %i\n", GetImageColor(image, j, i).r, GetImageColor(image, j, i).g, GetImageColor(image, j, i).b, GetImageColor(image, j, i).a);

            pixels[idx(i,j,numPixelsWidth)].color = GetImageColor(image, j, i);



 

        }
    }
    UnloadImage(image);

    runMainLoop(pixels, numPixelsHeight, numPixelsWidth, pixelSize, mode);


    CloseWindow();

    return 0;
}
