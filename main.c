// 	 Implementation sources: http:// 	lodev.org/cgtutor/files/quickcg/quickcg.cpp (verline function)
// 							 http:// 	lodev.org/cgtutor/raycasting.html

// TODO: Slide back to normal bob on view when stopped.
// TODO: Slide back to normal weapon bob on view when stopped.

// TODO: Firing should keep hand onscreen for longer / just fire one.
// TODO: Fix 1-pixel border on walls when close-up.
// TODO: Stars turn while turning.
// TODO: Graves

// TODO: Chunky status bar.

// Nice code formatting.
// Split off sprite stuff into own file.
// Split off wall drawing into own file.

// TODO: Pushing against wall should have more than {0,0} origin.
// TODO: Fireball should animate.
// TODO: Fireball explosion animation.

// Digitised enemy sprite within the map (prop like table and chairs?)
// Lovecraftian-themed.

#include <math.h>
#include <SDL.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "drawing.h"
#include "input.h"
#include "mysdl.h"

#define mapWidth   24
#define mapHeight  24

typedef struct {
	char r;
	char g;
	char b;
} Color;

// SPRITES =====================================================

Uint32 buffer[HEIGHT][WIDTH]; // y-coordinate first because it works per scanline

#define texWidth 64
#define texHeight 64
#define numSprites 8

//1D Zbuffer
double ZBuffer[WIDTH];
//arrays used to sort the sprites
int spriteOrder[numSprites];
double spriteDistance[numSprites];

//function used to sort the sprites
void combSort(int* order, double* dist, int amount)
{
  // int gap = amount;
  // bool swapped = false;
  // while(gap > 1 || swapped)
  // {
  //   //shrink factor 1.3
  //   gap = (gap * 10) / 13;
  //   if(gap == 9 || gap == 10) gap = 11;
  //   if (gap < 1) gap = 1;
  //   swapped = false;
  //   for(int i = 0; i < amount - gap; i++)
  //   {
  //     int j = i + gap;
  //     if(dist[i] < dist[j])
  //     {
  //       std::swap(dist[i], dist[j]);
  //       std::swap(order[i], order[j]);
  //       swapped = true;
  //     }
  //   }
  // }
}

// Source: http://sdl.beuc.net/sdl.wiki/Pixel_Access
Uint32 getPixel(SDL_Surface *surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to retrieve */
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

    switch(bpp) {
    case 1:
        return *p;
        break;

    case 2:
        return *(Uint16 *)p;
        break;

    case 3:
        if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
        break;

    case 4:
        return *(Uint32 *)p;
        break;

    default:
        return 0;       /* shouldn't happen, but avoids warnings */
    }
}

int worldMap[mapWidth][mapHeight] = {
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 },
	{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 }
};

#define PI 3.141592
#define SPRITE_COUNT 16
static int h = HEIGHT;
static int w = WIDTH;
float bobx;
float bobxInc;
float boby;
float bobyInc;
float fireballX = 2.0f;
float fireballY = 2.0f;
float turnAngle = 0.0f;
double posX;
double posY;
double niceTurn = 0.0f;
int spriteInc = 0;

typedef struct {
	float x;
	float y;
	float angle;
	float speed;
} Sprite2;

void verLine(int x, int y1, int y2, Color color) {
	if(y2 < y1) {y1 += y2; y2 = y1 - y2; y1 -= y2;} 	// swap y1 and y2
	if(y2 < 0 || y1 >= h	|| x < 0 || x >= w) return; 	// no single point of the line is on screen
	if(y1 < 0) y1 = 0; 									// clip
	if(y2 >= w) y2 = h - 1; 							// clip

	for(int i=y1; i < y2; i++) {
		Coord c = { x, i };
		drawPixel(c, color.r, color.g, color.b);
	}
}

Color makeColor(char r, char g, char b) {
	Color c = { r, g, b };
	return c;
}

Sprite2 sprites2[SPRITE_COUNT] = {
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
	{ 0,0,0,0 },
};

void fire() {
	spriteInc++;
	if(spriteInc == SPRITE_COUNT-1) spriteInc = 0;

	sprites2[spriteInc].x = posX;
	sprites2[spriteInc].y = posY;
	sprites2[spriteInc].angle = niceTurn;
	sprites2[spriteInc].speed = 0.5f;
}

int main()
{
	posX = 18;								// x and y start position
	posY = 18;
	double dirX = -1, dirY = 0; 			// initial direction vector
	double planeX = 0, planeY = 0.66; 		// the 2d raycaster version of camera plane

	// Initial facing direction.
	double tmp = 1.4;
	double oldDirX1 = dirX;
	dirX =    dirX * cos(tmp) - dirY * sin(tmp);
	dirY = oldDirX1 * sin(tmp) + dirY * cos(tmp);
	double oldPlaneX1 = planeX;
	planeX =    planeX * cos(tmp) - planeY * sin(tmp);
	planeY = oldPlaneX1 * sin(tmp) + planeY * cos(tmp);
	niceTurn = tmp;

	double time = 0; 		// time of current frame
	double oldTime = 0; 	// time of previous frame

	initSdl();
	initDrawing();

	// Blit hand to surface using SDL.
	SDL_Surface* hand = SDL_LoadBMP("hand.bmp");
	SDL_Surface* handShoot = SDL_LoadBMP("hand-fire3.bmp");
	SDL_Surface* fireballImage = SDL_LoadBMP("fireball.bmp");

	while(running) {
		if(!due(time, 1000 / 24)) continue;

		inputSdl();
		drawingClearFrame();

		// Update sprite motion.
		for(int i=0; i < SPRITE_COUNT; i++) {
			if(sprites2[i].speed == 0) continue;
			sprites2[i].x += sprites2[i].speed * cos(sprites2[i].angle - PI);
			sprites2[i].y += sprites2[i].speed * sin(sprites2[i].angle - PI);
		}

	    // Bobbing.
	    if(move != 0) {
		    float bobxAmount = 0.2;
		    float bobxSpeed = 0.2;
		    bobx = cos(bobxInc += bobxSpeed) * bobxAmount;
		    if(bobxInc >= PI*2) {
		    	bobxInc = 0;			// reset X
		    }

		    float bobyAmount = 0.2;
		    float bobySpeed = 0.2;
		    boby = sin(bobyInc -= bobySpeed) * bobyAmount;
		    if(bobyInc <= -PI) {
		    	bobyInc = 0;
		    }
	    }

		for(int x = 0; x < w; x++) {
			// calculate ray position and direction
			double cameraX = 2 * x / (double)w - 1; // 	x-coordinate in camera space
			double rayPosX = posX;
			double rayPosY = posY;
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;

			// which box of the map we're in
			int mapX = (int)rayPosX;
			int mapY = (int)rayPosY;

			// length of ray from current position to next x or y-side
			double sideDistX;
			double sideDistY;

			// length of ray from one x or y-side to next x or y-side
			double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
			double perpWallDist;

      		// what direction to step in x or y-direction (either +1 or -1)
			int stepX;
			int stepY;

			int hit = 0; 	// was there a wall hit?
			int side; 	// was a NS or a EW wall hit?

			// calculate step and initial sideDist
			if (rayDirX < 0) {
				stepX = -1;
				sideDistX = (rayPosX - mapX) * deltaDistX;
			} else {
				stepX = 1;
				sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
			}
			if (rayDirY < 0) {
				stepY = -1;
				sideDistY = (rayPosY - mapY) * deltaDistY;
			} else {
				stepY = 1;
				sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
			}

			// perform DDA
			while (hit == 0) {
				// 	jump to next map square, OR in x-direction, OR in y-direction
				if (sideDistX < sideDistY) {
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				} else {
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				
				// 	Check if ray has hit a wall
				if (worldMap[mapX][mapY] > 0) hit = 1;
			}

			// Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
			if (side == 0) perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
			else		   perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

			//SET THE ZBUFFER FOR THE SPRITE CASTING
			ZBuffer[x] = perpWallDist; //perpendicular distance is used

			// Calculate height of line to draw on screen
			int lineHeight = (int)(h / perpWallDist);

			// calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + h / 2;
			if(drawStart < 0)drawStart = 0;
			int drawEnd = lineHeight / 2 + h / 2;
			if(drawEnd >= h)drawEnd = h - 1;

			// choose wall color
			Color color = { 255, 255, 0 };
			int block = worldMap[mapX][mapY];

			if(block == 1){
				color = makeColor(255,0,255);
				if(side == 1) color = makeColor(128,0,128);
			}else if(block == 2){
				color = makeColor(0,255,255);
				if(side == 1) color = makeColor(0,128,128);
			}else if(block == 3){
				color = makeColor(255,255,255);
				if(side == 1) color = makeColor(192,192,192);
			}else if(block == 4) {
				color = makeColor(0,0,255);
				if(side == 1) color = makeColor(0,0,128);
			}

			// draw the pixels of the stripe as a vertical line
			verLine(x, drawStart + boby*6, drawEnd + boby*6, color);
		}


// ===============================

    //SPRITE CASTING
    //sort sprites from far to close
    for(int i = 0; i < numSprites; i++) {
      spriteOrder[i] = i;
      spriteDistance[i] = ((posX - fireballX) * (posX - fireballX) + (posY - fireballY) * (posY - fireballY)); //sqrt not taken, unneeded
//      spriteDistance[i] = ((posX - sprite[i].x) * (posX - sprite[i].x) + (posY - sprite[i].y) * (posY - sprite[i].y)); //sqrt not taken, unneeded
    }
    combSort(spriteOrder, spriteDistance, numSprites);

    // Project and draw the sprites
    for(int i = 0; i < SPRITE_COUNT; i++) {
      //translate sprite position to relative to camera
      double spriteX = sprites2[i].x - posX;
      double spriteY = sprites2[i].y - posY;
      // double spriteX = sprite[spriteOrder[i]].x - posX;
      // double spriteY = sprite[spriteOrder[i]].y - posY;

      double invDet = 1.0 / (planeX * dirY - dirX * planeY); //required for correct matrix multiplication

      double transformX = invDet * (dirY * spriteX - dirX * spriteY);
      double transformY = invDet * (-planeY * spriteX + planeX * spriteY); //this is actually the depth inside the screen, that what Z is in 3D

      int spriteScreenX = (int)((w / 2) * (1 + transformX / transformY));

      //calculate height of the sprite on screen
      int spriteHeight = abs((int)h / (transformY)); //using "transformY" instead of the real distance prevents fisheye
      //calculate lowest and highest pixel to fill in current stripe
      int drawStartY = -spriteHeight / 2 + h / 2;
      if(drawStartY < 0) drawStartY = 0;
      int drawEndY = spriteHeight / 2 + h / 2;
      if(drawEndY >= h) drawEndY = h - 1;

      //calculate width of the sprite
      int spriteWidth = abs( (int) (h / (transformY)));
      int drawStartX = -spriteWidth / 2 + spriteScreenX;
      if(drawStartX < 0) drawStartX = 0;
      int drawEndX = spriteWidth / 2 + spriteScreenX;
      if(drawEndX >= w) drawEndX = w - 1;

      //loop through every vertical stripe of the sprite on screen
      for(int stripe = drawStartX; stripe < drawEndX; stripe++)
      {
        //the conditions in the if are:
        //1) it's in front of camera plane so you don't see things behind you
        //2) it's on the screen (left)
        //3) it's on the screen (right)
        //4) ZBuffer, with perpendicular distance
        if(transformY > 0 && stripe > 0 && stripe < w && transformY < ZBuffer[stripe])
        for(int y = drawStartY; y < drawEndY; y++) //for every pixel of the current stripe
        {
          int d = (y) * 256 - h * 128 + spriteHeight * 128; //256 and 128 factors to avoid floats
          int texX = (int)(256 * (stripe - (-spriteWidth / 2 + spriteScreenX)) * texWidth / spriteWidth) / 256;
          int texY = ((d * texHeight) / spriteHeight) / 256;

          // get pixel color from image, splitting up RGB components from integer value.
          Uint32 color = getPixel(fireballImage, texX, texY);
          char r = (color & 0xFF000000) >> 24;
          char g = (color & 0x00FF0000) >> 16;
          char b = (color & 0x0000FF00) >> 8;

          // only draw pixel if not transparent.
          if((color & 0x000000FF) != 0)  {
			Coord c = { stripe, y };
          	drawPixel(c, r,g,b);
          }
        }
      }
    }


// ===============================

 		// timing for input and FPS counter
 	    oldTime = time;
	    time = clock();
	    double frameTime = (time - oldTime) / 1000.0;  // frameTime is the time this frame has taken, in seconds

		drawingFrame();

		if(shooting) {
			blitSprite(handShoot, 125 + (frameTime * bobx), 130 + (frameTime * boby));
			fire();
		}else{
			blitSprite(hand, 200 + (frameTime * bobx), 190 + (frameTime * boby));
		}

		updateWindow();

    	double moveSpeed = frameTime * 0.003;  // the constant value is in squares/second
    	double rotSpeed = frameTime * 0.003;   // the constant value is in radians/second

    	// move forward if no wall in front of you
		if (move == 1) {
			if(worldMap[(int)(posX + dirX * moveSpeed)][(int)posY] == false) posX += dirX * moveSpeed;
			if(worldMap[(int)posX][(int)(posY + dirY * moveSpeed)] == false) posY += dirY * moveSpeed;
		}
		// move backwards if no wall behind you
		if (move == -1) {
			if(worldMap[(int)(posX - dirX * moveSpeed)][(int)posY] == false) posX -= dirX * moveSpeed;
			if(worldMap[(int)posX][(int)(posY - dirY * moveSpeed)] == false) posY -= dirY * moveSpeed;
		}

		// rotate to the right
		if (turn == 1) {
			// both camera direction and camera plane must be rotated
			double oldDirX = dirX;
			dirX =    dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
			dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

			double oldPlaneX = planeX;
			planeX =    planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
			planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);

			niceTurn -= rotSpeed;
		}
		// rotate to the left
		if (turn == -1) {
			// both camera direction and camera plane must be rotated
			double oldDirX = dirX;
			dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
			dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
			double oldPlaneX = planeX;
			planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
			planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);

			niceTurn += rotSpeed;
		}
	}
}