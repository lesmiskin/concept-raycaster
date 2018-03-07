#ifndef _DRAWING_H
#define _DRAWING_H

#include "common.h"

extern void drawPixel(Coord coord, uchar r, uchar g, uchar b);
extern void drawingClearFrame();
extern void drawingFrame();
extern void initDrawing();
extern void shutdownDrawing();

#endif