#include <minix/drivers.h>
#include "timer.h"
#include "pixmap.h"
#include "interface.h"
#include "Bitmap.h"
#include "Player.h"
#include "Game.h"




FILE* logfd=NULL;

void initLog(){
	logfd=fopen(LOG_PATH, "w");
	LOG("initLog", "loggin successfully initialized");
}

int main(int argc, char **argv) {

	initLog();
	LOG("main", "starting program");
	sef_startup();
	CarPlayer *p=newPlayer(10,10,pic1);
	initGame(0x114);
	Game* c=startGame();
	drawMenu(c);
	newMouse();
	while(1){
		updateMouse();
		drawMouse();
	}
	exitGame();
	return 0;
}

