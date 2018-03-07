#!/bin/bash
clear
gcc *.c -std=c11 -lm `sdl2-config --cflags --libs` -o wireframe2.bin && ./wireframe2.bin
#echo Type ./wireframe2.bin to run.
