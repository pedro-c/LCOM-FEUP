#include "Obstacles.h"
#include <stdio.h>

Obstacles *newObstacle(int x,int y, Bitmap* tp){

	Obstacles *Obs=(Obstacles*)malloc(sizeof(Obstacles));
	Obs->x=x;
	Obs->y=y;
	Obs->type=tp;
	return Obs;
}

void drawTrack(Obstacles *obs){
	drawBitmap(obs->type,obs->x,obs->y,ALIGN_LEFT);
}

void drawObstacle(Obstacles* obs){
	drawBitmap(obs->type,obs->x,obs->y,ALIGN_LEFT);
}
