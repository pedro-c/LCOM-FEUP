#ifndef __OBSTACLES_H
#define __OBSTACLES_H

#include "Bitmap.h"

typedef struct{
	int x,y; //Coordenadas do obstaculo
	Bitmap* type; // bitmap referente ao obstaculo
} Obstacles;

Obstacles *newObstacle(int x,int y, Bitmap* tp);

void drawTrack(Obstacles *obs);

void drawObstacle(Obstacles* obs);










#endif
