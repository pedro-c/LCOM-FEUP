#ifndef __GAMESTATE_H
#define __GAMESTATE_H

#include "Bitmap.h"
#include "Player.h"
#include "Collisions.h"
#include "score.h"
#include "Pokeball.h"

#define OBS_SIZE      3
#define POKE_SIZE     3

typedef struct{
	int y, complete,vel;
	Bitmap* background;
	Player* player;
	Score* score;
	Obstacles* obs_l[OBS_SIZE];
	Obstacles* obs_c[OBS_SIZE];
	Obstacles* obs_r[OBS_SIZE];
	Pokeball* poke_l[POKE_SIZE];
	Pokeball* poke_c[POKE_SIZE];
	Pokeball* poke_r[POKE_SIZE];
	unsigned counter; //conta as interrupções
	unsigned sec; //conta os segundos de jogo
} GameState;

GameState* newGameState();
int updateGameState(GameState* g,unsigned long scancode,int counter);
void drawGameState(GameState* g);
void deleteGameState(GameState* g);
void moveBackGround(GameState* g);
int checkEndGame(GameState *g);
void updateAllObstacles(GameState* g,int incSpeed);
void updateCounterGame(GameState* g);
void updateAllPokeballs(GameState* g,int incSpeed);
int checkPokeballsCollision(GameState* g);



#endif
