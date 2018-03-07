#ifndef _MYSDL_H
#define _MYSDL_H

#include <SDL.h>
#include "common.h"

#define CHANNEL_R 0
#define CHANNEL_G 1
#define CHANNEL_B 2
#define CHANNELS 3              // 1 = Black and white. 3 = RGB.
#define PITCH CHANNELS * WIDTH  // 3 bytes per pixel * pixels per scanline / row.

SDL_Window* window;
SDL_Renderer* renderer;

extern void blitPixelBuffer(uchar *pixels);
extern void initSdl();
extern void shutdownSdl();

#endif