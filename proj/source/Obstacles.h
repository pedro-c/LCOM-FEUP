#ifndef __OBSTACLES_H
#define __OBSTACLES_H

#include "Bitmap.h"

typedef struct{
	int x,y; //Coordenadas do obstaculo
	Bitmap* type; // bitmap referente ao obstaculo
	int ht,wh;
	int use; //igual a 0 quando nao esta a ser usado e igual 1 quando esta a ser usado
	int vel;
} Obstacles;

Obstacles *newObstacle(int x,int y);

void updateObstacle(Obstacles* obs);

void drawObstacle(Obstacles* obs);

void deleteObstacle(Obstacles* obs);

void updateObstacleUse(Obstacles* obs);

void updateObstacleSpeed(Obstacles* obs,int incSpeed);//incSpeed=1 para aumentar a velocidade


#endif
