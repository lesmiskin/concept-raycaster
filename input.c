#include <SDL.h>
#include "common.h"
#include "input.h"

int turn = 0;
int move = 0;
bool shooting = false;

void inputSdl() {
    // Pump
    SDL_PumpEvents();
    SDL_Event event;
    const Uint8 *keysHeld = SDL_GetKeyboardState(NULL);
	
    while(SDL_PollEvent(&event) != 0) {
                
        // SDL events (e.g. clicking close button)
		switch(event.type) {
			case SDL_QUIT:
				running = false;
				return;
            // Pressed keys.
            case SDL_KEYDOWN: {
                //Ignore held keys.
                if (event.key.repeat) break;

                switch (event.key.keysym.scancode) {
                    case SDL_SCANCODE_ESCAPE:
                        running = false;
                        return;
                }
            }
		}
    }

    turn = 0; move = 0; shooting = false;
    if(keysHeld[SDL_SCANCODE_LEFT])  turn = -1;
    if(keysHeld[SDL_SCANCODE_RIGHT]) turn = 1;
    if(keysHeld[SDL_SCANCODE_UP])    move = 1;
    if(keysHeld[SDL_SCANCODE_DOWN])  move = -1;
    if(keysHeld[SDL_SCANCODE_SPACE])  shooting = true;
}
