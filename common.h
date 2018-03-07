#ifndef _COMMON_H
#define _COMMON_H

#include <stdbool.h>

#define WIDTH 320
#define HEIGHT 240
#define SCALE_FACTOR 3
#define W_WIDTH WIDTH * SCALE_FACTOR
#define W_HEIGHT HEIGHT * SCALE_FACTOR

typedef unsigned char uchar;

extern bool running;

typedef struct {
    float x;
    float y;
} Coord;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} Coord3;

extern long ticsToMilliseconds(long tics);
extern bool due(long compareTime, double milliseconds);
extern bool timer(long *lastTime, double hertz);
extern bool chance(float probability);

#endif