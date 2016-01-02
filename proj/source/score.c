#include "score.h"
#include "Bitmap.h"

Score* loadScoreFont() {
	Score* s = (Score*) malloc(sizeof(Score));

	s->score1=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score1.bmp");
	s->score2=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score2.bmp");
	s->score3=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score3.bmp");
	s->score4=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score4.bmp");
	s->score5=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score5.bmp");
	s->score6=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score6.bmp");
	s->score7=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score7.bmp");
	s->score8=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score8.bmp");
	s->score9=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score9.bmp");
	s->score0=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/score0.bmp");
	s->scoreBoard=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/scoreboard.bmp");
	return s;
}

void scoreDisplay(Score* s, int score){

	drawBitmap(s->scoreBoard,5,18,ALIGN_LEFT);
	memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
	char scoreStr[1];
	if(score<10){
		char scoreStr[2];
	}
	else{
		char scoreStr[6];
	}

	sprintf(scoreStr, "%d", score);
	int x=18;
	int y=55;
	int i;
	for (i = 0; i<6; i++){
		char number=scoreStr[i];
		switch (number) {
			case '1':
				drawBitmap(s->score1,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '2':
				drawBitmap(s->score2,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '3':
				drawBitmap(s->score3,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '4':
				drawBitmap(s->score4,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '5':
				drawBitmap(s->score5,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '6':
				drawBitmap(s->score6,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '7':
				drawBitmap(s->score7,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '8':
				drawBitmap(s->score8,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '9':
				drawBitmap(s->score9,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			case '0':
				drawBitmap(s->score0,x,y,ALIGN_LEFT);
				memcpy(getGraphicsBuffer(),getGraphicsBufferTmp(),getVRAMSize());
				break;
			default:
				break;
			}
		x+=10;
	}

}
