#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

#define WIDTH 640
#define HEIGHT 480

typedef unsigned char uchar;

typedef struct {
    float x;
    float y;
} Vect2;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Vect3;

extern bool running;

extern Vect2 makeVect2(float x, float y);
extern Vect3 makeVect3(float x, float y, float z, float w);
extern long ticsToMilliseconds(long tics);
extern bool due(long compareTime, double milliseconds);
extern bool timer(long *lastTime, double hertz);

#endif