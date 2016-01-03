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

void flipDisplay(char* vm);

char *read_xpm(char *map[], int *wd, int *ht);

int print_xpm(unsigned short xi, unsigned short yi, char *xpm[]);

void refresh();

#endif
