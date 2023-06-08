#include <stdio.h>
#include <time.h>
#include "utils.h"
#include "energies.h"

#define COMPLETE_RANDOM 0
#define EUCLIDIAN_RIGHT_DOWN 1
#define STRAIGHT_VERTICAL 2
#define LOOK_LIKE_IMAGE 3
#define EUCLIDIAN_UNIFORM 4
#define SPIRAL 5
/*
#define RAYGUI_IMPLEMENTATION
#include "./raygui.h"

#undef RAYGUI_IMPLEMENTATION     
*/




void runMainLoop(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth, float pixelSize, int mode, bool* canReset) {
    //BeginDrawing();
    Image image = LoadImage("images/logo32.png");  // Load image data into CPU memory (RAM)
    
    float temperature = 150;
    float decayFactor = 0.999;
    float finalTemperature = 0;

    int iteration = 0;
    int freqDisplay = 50;

    CustomPixel* neighbourPixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);
    copyArrayTo(neighbourPixels, pixels, numPixelsHeight, numPixelsWidth);

    float delta = 0;
    float p = 0;

    Couple lastModified = (Couple){ .p1 = (Point){ .i = 0, .j = 0}, .p2 = (Point){ .i = 0, .j = 0} };
    bool shouldModify = false;

    while (temperature > finalTemperature) {
        if (IsKeyUp(KEY_R)) {
            *canReset = true;
        }

        if (*canReset && IsKeyDown(KEY_R)) {
            *canReset = false;
            break;
        }

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
        case 3:
            delta = look_like_image_energy(pixels, image, numPixelsHeight, numPixelsWidth) - look_like_image_energy(neighbourPixels, image, numPixelsHeight, numPixelsWidth);
            break;
        case 4:
            delta = euclidian_uniform(pixels, numPixelsHeight, numPixelsWidth) - euclidian_uniform(neighbourPixels, numPixelsHeight, numPixelsWidth);
            break;
        case 5:
            delta = spiral_energy(pixels, numPixelsHeight, numPixelsWidth) - spiral_energy(neighbourPixels, numPixelsHeight, numPixelsWidth);
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
    UnloadImage(image);
    free(neighbourPixels);
    return;
    //EndDrawing();
}

int main(void) {
    srand(time(NULL));

    const int screenWidth = 512;
    const int screenHeight = 512;

    bool canReset = true;

    // Initialisation des paramètres
    
    float pixelSize = 8.0;

    int mode = SPIRAL;


    InitWindow(screenWidth, screenHeight, "Exemple de fenêtre avec paramètres");

    int numPixelsWidth = screenWidth / pixelSize;
    int numPixelsHeight = screenHeight / pixelSize;



    CustomPixel* pixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);



    while (true) {
        Image image = LoadImage("images/logo64.png");  // Load image data into CPU memory (RAM)
    
        for (int i = 0; i < numPixelsHeight; i++) {
            for (int j = 0; j < numPixelsWidth; j++) {
                pixels[idx(i,j,numPixelsWidth)].i = i;
                pixels[idx(i,j,numPixelsWidth)].j = j;
                // Initialisation des pixels personnalisés avec des couleurs aléatoires
                
                //int random1 = GetRandomValue(0, 255);
                //int random2 = GetRandomValue(0, 255);
                //int random3 = GetRandomValue(0, 255);
                //pixels[idx(i,j,numPixelsWidth)].color = (Color){random1, random1, random1, 255};
                

                // Chargment image
                //printf("Color : %i %i %i %i\n", GetImageColor(image, j, i).r, GetImageColor(image, j, i).g, GetImageColor(image, j, i).b, GetImageColor(image, j, i).a);

                pixels[idx(i,j,numPixelsWidth)].color = GetImageColor(image, j, i);
            }
        }
        UnloadImage(image);
        runMainLoop(pixels, numPixelsHeight, numPixelsWidth, pixelSize, mode, &canReset);
    }


    CloseWindow();

    return 0;
}
