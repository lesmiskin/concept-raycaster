#ifndef _SHAPES_H
#define _SHAPES_H

#include "common.h"
#include "drawing.h"

typedef struct {
    Vect3* points;
    int numPoints;
    float scale;
    Vect3 origin;
    Vect3 angle;
} Shape;

typedef struct {
	Vect3 origin;
} Block;

#endif