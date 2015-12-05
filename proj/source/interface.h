#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "MainMenuState.h"

int exitGame();

void *initGame(unsigned short mode);

void fill_pixel(unsigned short x, unsigned short y, unsigned long color);

unsigned getVerResolution();

unsigned getHorResolution();

unsigned getGraphicsBuffer();

void flipDisplay(char* vm);

const char* getImagePath(const char* image);

//int drawRectangle(long xi, long yi, long xf, long yf, int color);

//int drawRect(Rectangle* rect, int color);


#endif
