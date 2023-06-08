#ifndef DEF
#define DEF

#include <raylib.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>


// Structure pour représenter la couleur personnalisée d'un pixel
typedef struct {
    int i;
    int j;
    Color color;
} CustomPixel;

typedef struct {
    int i;
    int j;
} Point;

typedef struct {
    Point p1;
    Point p2;
} Couple;

#endif