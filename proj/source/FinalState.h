#ifndef __FINALSTATE_H
#define __FINALSTATE_H

#include <stdio.h>
#include "Button.h"
#include "Bitmap.h"
#include "fonts.h"

typedef struct{

	Fonts* f;
	Bitmap* background;
	Bitmap* HighScoresBackground;
	int score;
	int play;
	int scoreTable;
	FILE *file;
	char first[30];
	char second[30];
	char third[30];
	char firstName[30];
	char secondName[30];
	char thirdName[30];
	char firstDate[30];
	char secondDate[30];
	char thirdDate[30];
	char newHighScore[30];
	char newHighScoreName[30];
	char newHighScoreDate[30];
	int flag;

}FinalState;

FinalState* newFinalState();
int updateFinalState(FinalState* state, unsigned long scancode, int score, int scoreAux);
void readHighScores(FinalState* state);
void printHighScores(FinalState* state);
void newHighScore(FinalState* state);
int verifyFinalStateChange(FinalState* state);
void drawFinalMenu(FinalState* state);
void deleteFinalState(FinalState* state);



#endif
