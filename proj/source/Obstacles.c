#include "Obstacles.h"
#include <stdio.h>

Obstacles *newObstacle(int x,int y, Bitmap* tp){

	Obstacles *Obs=(Obstacles*)malloc(sizeof(Obstacles));
	Obs->x=x;
	Obs->y=y;
	Obs->type=tp;
	return Obs;
}

void drawTrack(Obstacles *obs1,Obstacles *obs2){
	drawBitmap(obs1->type,obs1->x,obs1->y,ALIGN_LEFT);
	drawBitmap(obs2->type,obs2->x,obs2->y,ALIGN_LEFT);
}

void drawObstacle(Obstacles* obs){
	drawBitmap(obs->type,obs->x,obs->y,ALIGN_LEFT);
}
