#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "Bitmap.h"
#include "Player.h"
#include "Collisions.h"

#define OBS_SIZE      3

typedef struct{
	int y, complete;
	Bitmap* background;
	Player* player;
	Obstacles* obs_l[OBS_SIZE];
	Obstacles* obs_c[OBS_SIZE];
	Obstacles* obs_r[OBS_SIZE];
	unsigned counter; //conta as interrupções
	unsigned sec; //conta os segundos de jogo
} GameState;

GameState* newGameState();
void updateGameState(GameState* g,unsigned char* keyCode);
void drawGameState(GameState* g);
void deleteGameState(GameState* g);
void moveBackGround(GameState* g);
int checkEndGame(GameState *g);



#endif
