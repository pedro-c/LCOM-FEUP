#ifndef __SCORE_H
#define __SCORE_H

#include "Bitmap.h"

typedef struct{

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

Score* loadScoreFont();
void scoreDisplay(Score* s,int score);

#endif
