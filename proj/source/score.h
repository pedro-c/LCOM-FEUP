#ifndef __SCORE_H
#define __SCORE_H

#include "Bitmap.h"

typedef struct{
	unsigned int score;
	Bitmap* score1;
	Bitmap* score2;
	Bitmap* score3;
	Bitmap* score4;
	Bitmap* score5;
	Bitmap* score6;
	Bitmap* score7;
	Bitmap* score8;
	Bitmap* score9;
	Bitmap* score0;
	Bitmap* scoreBoard;
}Score;

Score* newScore();
void updateScore(Score* s,int hitPoke);//hitPoke=0 se nao atingiu e hitPoke=1 atingiu pokeball
void drawScore(Score* s);
unsigned int returnScore(Score* s);
void deleteScore(Score* s);

#endif
