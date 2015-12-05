#include <minix/drivers.h>
#include "timer.h"
#include "pixmap.h"
#include "interface.h"
#include "Bitmap.h"
#include "Player.h"
#include "Game.h"

int main(int argc, char **argv) {

	sef_startup();
	CarPlayer *p=newPlayer(10,10,pic1);
	initGame(0x114);
	movePlayer(p);
	//Game* c=startGame();
	//drawMenu(c);
	exitGame();
	return 0;
}

