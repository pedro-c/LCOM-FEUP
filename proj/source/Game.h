#ifndef __GAME_H
#define __GAME_H

#include <stdio.h>
#include "MainMenuState.h"


typedef enum{
	MAIN_MENU_STATE, GAME_STATE, FINAL_STATE
}State;


typedef struct{

	int set_kb;
	int set_mouse;
	int set_timer;
	int counter;
	State currentState;
	void* state;
	unsigned long scancode;
	int mainScore;


}Game;

Game* startGame();
int updateGame(Game* game);
void drawGame(Game* game);
void deleteState(Game* game);
void deleteGame(Game* game);

#endif
