#ifndef UTILS
#define UTILS

#include "def.h"

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
    BeginDrawing();
    ClearBackground(BLACK);

    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            DrawCustomPixel(&pixels[idx(i,j,numPixelsWidth)], pixelSize);
        }
    }
    EndDrawing();
}

void copyArrayTo(CustomPixel* dst, CustomPixel* src, int numPixelsHeight, int numPixelsWidth) {
    for (int i = 0; i < numPixelsHeight; i++) {
        for (int j = 0; j < numPixelsWidth; j++) {
            dst[idx(i,j,numPixelsWidth)] = src[idx(i,j,numPixelsWidth)];
        }
    }
}

void changePixelInArray(CustomPixel* dst, CustomPixel* src, Point p, int numPixelsWidth) {
    dst[idx(p.i,p.j,numPixelsWidth)] = src[idx(p.i,p.j,numPixelsWidth)];
}

void changeCouplePixelInArray(CustomPixel* dst, CustomPixel* src, Couple cpl, int numPixelsWidth) {
    changePixelInArray(dst, src, cpl.p1, numPixelsWidth);
    changePixelInArray(dst, src, cpl.p2, numPixelsWidth);
}

float randomFloat()
{
      float r = (float)rand()/(float)RAND_MAX;
      return r;
}

Couple swapRandomPixels(CustomPixel* pixels, int numPixelsHeight, int numPixelsWidth) {
    // Renvoie les points échangés
    
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

    return (Couple){ .p1 = (Point){ .i = i, .j = j}, .p2 = (Point){ .i = k, .j = l} };
}

unsigned char getColorFromId(int id, CustomPixel pix) {
    switch (id)
    {
    case 0:
        return pix.color.r;
    case 1:
        return pix.color.g;
    case 2:
        return pix.color.b;
    default:
        return 0;
    }
}

void changeColorFromId(int id, CustomPixel* pix, unsigned char newCol) {
    switch (id)
    {
    case 0:
        pix->color.r = newCol;
        break;
    case 1:
        pix->color.g = newCol;
        break;
    case 2:
        pix->color.b = newCol;
        break;
    default:
        break;
    }
}

Couple swapRandomPixelsColors(CustomPixel* pixels, int numPixelsHeight, int numPixelsWidth) {
    // Renvoie les pixels échangés
    
    int i = GetRandomValue(0, numPixelsHeight - 1);
    int k = GetRandomValue(0, numPixelsHeight - 1);
    int j = GetRandomValue(0, numPixelsWidth - 1);
    int l = GetRandomValue(0, numPixelsWidth - 1);
    
    int col1 = GetRandomValue(0, 2);
    int col2 = GetRandomValue(0, 2);

    unsigned char temp = getColorFromId(col1, pixels[idx(i,j, numPixelsWidth)]);
    changeColorFromId(col1, &pixels[idx(i,j, numPixelsWidth)], getColorFromId(col2, pixels[idx(k,l, numPixelsWidth)]));
    changeColorFromId(col2, &pixels[idx(k,l, numPixelsWidth)], temp);

    return (Couple){ .p1 = (Point){ .i = i, .j = j}, .p2 = (Point){ .i = k, .j = l} };

}


#endif