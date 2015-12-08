#ifndef __GAME_H
#define __GAME_H

#include "Bitmap.h"
#include "timer.h"
#include <stdio.h>

char msg, tag, var;
//log data
extern FILE* logfd;
#define LOG_PATH "home/lcom/lcom1516-t2g12/proj/log.txt"
#define LOG(tag, msg) fprintf(logfd,"%s: %d/n", tag, var)
#define LOG_VAR(tag,var) fprintf(logfd, "%s: %d/n", tag, var)





typedef struct{
	int subscribe_KB;
	int subscribe_MOUSE;
	int subscribe_TIMER;

	Bitmap* menu;

}Game;

Game* startGame();
void drawMenu(Game* g);

#endif
