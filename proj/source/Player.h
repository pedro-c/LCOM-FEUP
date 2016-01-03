#ifndef __PLAYER_H
#define __PLAYER_H

#include "interface.h"
#include "keyboard.h"
#include "Obstacles.h"
#include "Bitmap.h"

typedef struct{
	int x,y,vel,vel_i; //coordenadas do jogador,velocidade variavel  e velocidade constante
	Bitmap* ash_l;
	Bitmap* ash_r;
	int hPlayer, wPlayer; //width e height
	int counter,sec,change;
}Player;

Player* newPlayer();

void drawPlayer(Player* p);

void movePlayer(Player* p,unsigned long scancode);

void deletePlayer(Player* p);

int checkTrackCollision(Player* p);

void updateCounterPlayer(Player* p);

void updatePlayerSpeed(Player* p,int incSpeed);

#endif

