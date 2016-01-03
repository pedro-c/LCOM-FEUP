#ifndef __POKEBALL_H
#define __POKEBALL_H

#include "Bitmap.h"

typedef struct{
	int x,y; //Coordenadas do obstaculo
	Bitmap* poke; // bitmap referente ao obstaculo
	int ht,wh;
	int use; //igual a 0 quando nao esta a ser usado e igual 1 quando esta a ser usado
	int val; //igual a 1 quando ainda nao foi apanhada e igual a 0 quando ja foi apanhada
	int vel;
} Pokeball;

Pokeball *newPoke(int x,int y);

void updatePoke(Pokeball* pb);

void drawPoke(Pokeball* pb);

void deletePoke(Pokeball* pb);

void updatePokeUse(Pokeball* pb);

void updatePokeSpeed(Pokeball* pb,int incSpeed);




#endif
