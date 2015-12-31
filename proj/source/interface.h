#ifndef __INTERFACE_H
#define __INTERFACE_H

#include "MainMenuState.h"

int exitGame();

void *initGame(unsigned short mode);

void fill_pixel(unsigned short x, unsigned short y, unsigned short color);

unsigned getVerResolution();

unsigned getHorResolution();

char* getGraphicsBuffer();

char* getGraphicsBufferTmp();

unsigned getVRAMSize();

unsigned getBytesPerPixel();

int print_Bitmap(unsigned char* data,int x, int y,int ht,int wt);

void flipDisplay(char* vm);

const char* getImagePath(const char* image);

#endif
