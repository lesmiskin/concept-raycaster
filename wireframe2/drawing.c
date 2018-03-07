#include "drawing.h"
#include "mysdl.h"

static uchar* bg;
static uchar* pixels;
static int bufferSize;

void drawPixel(Vect2 coord, uchar r, uchar g, uchar b) {
    // The whole thing is stored as a *sequential array*.
    // Can specify byte values in hex, too. e.g. 0xff

    int x = coord.x;
    int y = coord.y;

    // Don't draw if out of bounds.
    if(x > WIDTH || x < 0 || y > HEIGHT || y < 0) return;

    // Do that funky math, white boy.
    int doX = x > 0 ? x * CHANNELS : 1;     // x times each block of 3x RGB values.
    int doY = y > 0 ? (y * PITCH) : 1;      // y times each scanline.
    int index = doX + doY;                  // Add both X and Y values to arrive at the right index.

    pixels[index + CHANNEL_R] = r;
    pixels[index + CHANNEL_G] = g;
    pixels[index + CHANNEL_B] = b;
}

void drawingClearFrame() {
    // Copy background on each frame.
    memcpy(pixels, bg, bufferSize);
}

void drawingFrame() {
    blitSdl(pixels);
}

void drawBg() {
    int full = sizeof(uchar) * WIDTH  * HEIGHT * CHANNELS;

    for(int i=0; i < full; i += 3) {
        // Ceiling
        if(i < full/2){
            bg[i + CHANNEL_R] = 0;
            bg[i + CHANNEL_G] = 0;
            bg[i + CHANNEL_B] = 0;
        // Floor
        } else {
            bg[i + CHANNEL_R] = 84;
            bg[i + CHANNEL_G] = 86;
            bg[i + CHANNEL_B] = 84;
        }
    }
}

void initDrawing() {
    bufferSize = sizeof(uchar) * WIDTH  * HEIGHT * CHANNELS;

    // The main pixel array
    bg = malloc(bufferSize);
    pixels = malloc(bufferSize);

    drawBg();
}

void shutdownDrawing() {
    free(pixels);
    pixels = NULL;
}