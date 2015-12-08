#ifndef __GAME_H
#define __GAME_H

#include "Bitmap.h"
#include "timer.h"

typedef struct{
	int subscribe_KB;
	int subscribe_MOUSE;
	int subscribe_TIMER;

	Bitmap* menu;

}Game;

Game* startGame();
void drawMenu(Game* g);

#endif
