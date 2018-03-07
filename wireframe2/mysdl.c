#include "mysdl.h"
#include "common.h"

#define DEPTH 8 * CHANNELS      // How many BITS per pixel we have. We have 3 channels, each with one 8-bit byte. So 8 * 3.

static const Uint32 RMASK = 0x0000ff;
static const Uint32 GMASK = 0x00ff00;
static const Uint32 BMASK = 0xff0000;
static const Uint32 AMASK = 0;

static const char* GAME_TITLE = "ASTEROIDS © 1979, Atari";

void blitSdl(uchar *pixels) {
    // Make an SDL surface from our pixel data.
    SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels, WIDTH, HEIGHT, DEPTH, PITCH, RMASK, GMASK, BMASK, AMASK);

    // Copy the SDL surface to the window.
    SDL_Surface* windowSurface = SDL_GetWindowSurface(window);
    SDL_BlitSurface(surface, NULL, windowSurface, NULL);
    SDL_UpdateWindowSurface(window);

    // Empty resources used by our SDL surface.
    SDL_FreeSurface(surface);
}

void initSdl() {
    // Init SDL proper.
    SDL_Init(SDL_INIT_VIDEO);

    // Make SDL window.
    window = SDL_CreateWindow(
        GAME_TITLE,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIDTH,
        HEIGHT,
        0                           // SDL_WINDOW_XYZ flags
    );

    // Make SDL renderer.
    renderer = SDL_CreateRenderer(
        window,
        -1,                                       // insert at default index position for renderer list.
        0
//        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
}

void shutdownSdl() {
    // Destroy renderer.
    if(renderer == NULL) return;			// OK to call if not yet setup (thanks, encapsulation)
    SDL_DestroyRenderer(renderer);
    renderer = NULL;

    // Destroy window.
    if(window == NULL) return;			    // OK to call if not yet setup (an established subsystem pattern elsewhere)
    SDL_DestroyWindow(window);
    window = NULL;

    // Destroy SDL proper.
    SDL_Quit();
}
