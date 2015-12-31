#ifndef __MAINMENUSTATE_H
#define __MAINMENUSTATE_H

#include <stdio.h>
#include "Bitmap.h"
#include "mouse.h"
#include "Button.h"

typedef enum{
	PLAY_CHOSEN, EXIT_CHOSEN
}MainMenuAction;

typedef struct{

	int play;
	int overPlay;
	int overExit;

	Bitmap* menuImage;
	Bitmap* pokeball;
	Box* playButton;
	Box* exitButton;


}MainMenuState;

MainMenuState* newMainMenuState();
void drawPokeball(MainMenuState* state);
int verifyStateChange(MainMenuState* state);
int updateMainMenuState(MainMenuState* state,  unsigned long scancode);
void drawMenu(MainMenuState* state);
void deleteMainMenuState(MainMenuState* state);

#endif
