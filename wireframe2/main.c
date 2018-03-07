
#include <math.h>
#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>

#include "algorithms.h"
#include "common.h"
#include "drawing.h"
#include "input.h"
#include "mysdl.h"

static int FPS = 60;

void makeScene() {

}

void renderScene() {

}

static void shutdownMain() {
    shutdownDrawing();
    shutdownSdl();
}

int main() {
	// Initialise subsystems.
	initSdl();
	initDrawing();

    atexit(shutdownMain);

	//Main game loop.
	long lastRenderFrameTime = clock();

	makeScene();

	while(running){
		if(!timer(&lastRenderFrameTime, 1000 / FPS)) continue;

		inputSdl();
		drawingClearFrame();

		renderScene();

		// (blitting)
		drawingFrame();
	}

	return 0;
}