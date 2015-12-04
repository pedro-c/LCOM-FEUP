#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

int exitGame();

void *initGame(unsigned short mode);

void fill_pixel(unsigned short x, unsigned short y, unsigned long color);

unsigned getVerResolution();

unsigned getHorResolution();

unsigned getGraphicsBuffer();

void flipDisplay(char* vm);
#endif
