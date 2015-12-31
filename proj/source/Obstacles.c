#include "Obstacles.h"

#include <time.h>

Obstacles *newObstacle(int x,int y){

	Obstacles *Obs=(Obstacles*)malloc(sizeof(Obstacles));
	Obs->x=x;
	Obs->y=y;
	Obs->use=0;
	Obs->type=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/obs120.bmp");
	Obs->ht=Obs->type->bitmapInfoHeader.height;
	Obs->wh=Obs->type->bitmapInfoHeader.width;
	return Obs;
}

void updateObstacle(Obstacles* obs){
	obs->y+=3;
	if(obs->y>=getVerResolution())
		{
		obs->y=0;
		obs->use=rand()%2;
		}
}

void drawObstacle(Obstacles* obs){
	drawBitmap(obs->type,obs->x,obs->y,ALIGN_LEFT);
}

void deleteObstacle(Obstacles* obs){
	if(obs==NULL)
		return;
	deleteBitmap(obs->type);
	free(obs);
}

void updateObstacleUse(Obstacles* obs){
	obs->use=rand()%2;
}
