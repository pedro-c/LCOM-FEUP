#ifndef __PLAYER_H
#define __PLAYER_H

#include "Bitmap.h"

typedef struct{
	int x,y; //coordenadas do carro
	int verfifyCrash; //1-Carro colidiu,0- Não colidiu
	Bitmap* car;
}CarPlayer;

CarPlayer* newPlayer(int x,int y);

void setCoordinates(CarPlayer* p,int x,int y);

void drawPlayer(CarPlayer* p);

void movePlayer(CarPlayer* p);

#endif
