#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "drawing.h"
#include "input.h"

// Source: http://alienryderflex.com/polygon_fill/
void fillPolygon(int numPoints, Vect2* points, uchar r, uchar g, uchar b) {
    int nodes;
    int nodeX[numPoints];
    int pixelX;
    int pixelY;
    int swap;
    int i, j;

    int IMAGE_LEFT =  points[i].x;
    int IMAGE_RIGHT = points[i].x;
    int IMAGE_TOP =   points[i].y;
    int IMAGE_BOT =   points[i].y;

    // Establish analytical bounding box for polygon to improve efficiency.
    for(i=0; i < numPoints; i++) {
        if(points[i].x < IMAGE_LEFT)  IMAGE_LEFT =  points[i].x;
        if(points[i].x > IMAGE_RIGHT) IMAGE_RIGHT = points[i].x;
        if(points[i].y < IMAGE_TOP)   IMAGE_TOP =   points[i].y;
        if(points[i].y > IMAGE_BOT)   IMAGE_BOT =   points[i].y;
    }

    // If one of the bounds is offscreen - limit filling calcs to screen edge.
    if(IMAGE_LEFT <= 0)      IMAGE_LEFT = 0;
    if(IMAGE_RIGHT >= WIDTH) IMAGE_RIGHT = WIDTH;
    if(IMAGE_TOP <= 0)       IMAGE_TOP = 0;
    if(IMAGE_BOT >= HEIGHT)  IMAGE_BOT = HEIGHT;

    //  Loop through the rows of the image.
    for (pixelY = IMAGE_TOP; pixelY < IMAGE_BOT; pixelY++) {
        //  Build a list of nodes.
        nodes = 0; 
        j = numPoints - 1;
        for (i = 0; i < numPoints; i++) {
            if (points[i].y < (double) pixelY && points[j].y >= (double) pixelY ||
                points[j].y < (double) pixelY && points[i].y >= (double) pixelY) {
                nodeX[nodes++] = (int)(points[i].x + (pixelY - points[i].y) / (points[j].y - points[i].y) * (points[j].x - points[i].x)); 
            }
            j=i; 
        }

        //  Sort the nodes, via a simple “Bubble” sort.
        i=0;
        while (i < nodes - 1) {
            if (nodeX[i] > nodeX[i+1]) {
                swap = nodeX[i]; 
                nodeX[i] = nodeX[i+1]; 
                nodeX[i+1] = swap; 
                if (i) i--; 
            } else {
                i++; 
            }
        }

        //  Fill the pixels between node pairs.
        for (i = 0; i < nodes; i += 2) {
            if (nodeX[i] >= IMAGE_RIGHT) break;
            if (nodeX[i+1] > IMAGE_LEFT) {
                if (nodeX[i] < IMAGE_LEFT) nodeX[i] = IMAGE_LEFT;
                if (nodeX[i+1] > IMAGE_RIGHT) nodeX[i+1] = IMAGE_RIGHT;
                for (pixelX = nodeX[i]; pixelX < nodeX[i+1]; pixelX++)  {
                    Vect2 c = { pixelX, pixelY };
                    drawPixel(c, r,g,b); 
                }
            }
        }
    }
}
