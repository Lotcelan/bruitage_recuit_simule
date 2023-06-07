#include <stdio.h>
#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>


#define COMPLETE_RANDOM 0
#define GAUSSIAN 1

/*
#define RAYGUI_IMPLEMENTATION
#include "./raygui.h"

#undef RAYGUI_IMPLEMENTATION     
*/

// Structure pour représenter la couleur personnalisée d'un pixel
typedef struct {
    int i;
    int j;
    Color color;
} CustomPixel;

// Fonction pour afficher un pixel personnalisé
void DrawCustomPixel(const CustomPixel* pixel, int pixelSize) {
    Rectangle pixelRect = {
        pixel->j * pixelSize,
        pixel->i * pixelSize,
        pixelSize,
        pixelSize
    };
    DrawRectangleRec(pixelRect, pixel->color);
}

int idx(int i, int j, int numPixelsWidth) {
    return i*numPixelsWidth + j;
}

void draw_all_pixels(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth, int pixelSize) {
    //BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            DrawCustomPixel(&pixels[idx(i,j,numPixelsWidth)], pixelSize);
        }
    }
    EndDrawing();
}
void complete_random_mode(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth, float pixelSize) {
    
    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            pixels[idx(i, j, numPixelsWidth)].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
        }
    }
    //draw_all_pixels(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
}

float euclidian_color_dist(Color col1, Color col2) {
    return sqrt( pow(col1.r - col2.r, 2) + pow(col1.g - col2.g, 2) + pow(col1.b - col2.b, 2));
}

float gaussian_energy(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth) {
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

void copyArrayTo(CustomPixel* dst, CustomPixel* src, int numPixelsHeight, int numPixelsWidth) {
    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            dst[idx(i,j,numPixelsWidth)] = src[idx(i,j,numPixelsWidth)];
        }
    }
}

float randomFloat()
{
      float r = (float)rand()/(float)RAND_MAX;
      return r;
}

void swapRandomPixelsColors(CustomPixel* pixels, int numPixelsHeight, int numPixelsWidth) {
    int i = GetRandomValue(0, numPixelsHeight - 1);
    int k = GetRandomValue(0, numPixelsHeight - 1);
    int j = GetRandomValue(0, numPixelsWidth - 1);
    int l = GetRandomValue(0, numPixelsWidth - 1);
    
    CustomPixel temp = pixels[idx(i,j, numPixelsWidth)];
    pixels[idx(i,j, numPixelsWidth)] = pixels[idx(k,l, numPixelsWidth)];
    pixels[idx(i,j, numPixelsWidth)].i = i;
    pixels[idx(i,j, numPixelsWidth)].j = j;
    pixels[idx(k,l, numPixelsWidth)] = temp;
    pixels[idx(k,l, numPixelsWidth)].i = k;
    pixels[idx(k,l, numPixelsWidth)].j = l;
}

void gaussian_mode(CustomPixel *pixels, int numPixelsHeight, int numPixelsWidth, float pixelSize) {
    BeginDrawing();
    float temperature = 1000;
    float decayFactor = 0.9999;
    float finalTemperature = 0;

    int iteration = 0;
    int freqDisplay = 100;

    CustomPixel* neighbourPixels = (CustomPixel*)malloc(sizeof(CustomPixel) * numPixelsHeight * numPixelsWidth);

    float delta = 0;
    float p = 0;

    while (temperature > finalTemperature) {
        iteration++;
        copyArrayTo(neighbourPixels, pixels, numPixelsHeight, numPixelsWidth);
        swapRandomPixelsColors(neighbourPixels, numPixelsHeight, numPixelsWidth);
        
        delta = gaussian_energy(pixels, numPixelsHeight, numPixelsWidth) -  gaussian_energy(neighbourPixels, numPixelsHeight, numPixelsWidth);
        //printf("Iteration : %i, delta actuel : %f, température actuelle : %f\n", iteration, delta, temperature);

        // On veut minimiser l'énergie
        if (delta >= 0) {
            copyArrayTo(pixels, neighbourPixels, numPixelsHeight, numPixelsWidth);
        } else {
            p = randomFloat();
            printf("Calcul chelou : %f, p = %f\n", exp(delta / temperature), p);
            if (exp(delta / temperature) >= p) {
                copyArrayTo(pixels, neighbourPixels, numPixelsHeight, numPixelsWidth);
            }
        }
        
        if (iteration == freqDisplay) {
            iteration = 0;
            draw_all_pixels(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
        }
        temperature *= decayFactor;
    }
    EndDrawing();
}

int main(void) {
    srand(time(NULL));

    const int screenWidth = 800;
    const int screenHeight = 600;

    // Initialisation des paramètres
    
    float pixelSize = 40.0f;

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
            pixels[idx(i,j,numPixelsWidth)].color = (Color){GetRandomValue(0, 255), GetRandomValue(0, 255), GetRandomValue(0, 255), 255};
        }
    }


    /*
    while (!WindowShouldClose()) {


        BeginDrawing();
        ClearBackground(BLACK);
        draw_all_pixels(pixels, numPixelsHeight, numPixelsWidth, pixelSize);

        EndDrawing();

        switch (mode)
        {
        case 0:
            complete_random_mode(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
        case 1:
            gaussian_mode(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
        default:
            break;
        }

        

        // Afficher les pixels personnalisés
        

    } */

    switch (mode)
    {
    case 0:
        complete_random_mode(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
    case 1:
        gaussian_mode(pixels, numPixelsHeight, numPixelsWidth, pixelSize);
    default:
        break;
    }

    CloseWindow();

    return 0;
}
