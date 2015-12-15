#include <minix/drivers.h>
#include "timer.h"
#include "pixmap.h"
#include "interface.h"
#include "Bitmap.h"
#include "Player.h"
#include "Game.h"

int main(int argc, char **argv) {

	sef_startup();
	initGame(0x114);
	//Bitmap* p=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/pokemon.bmp");
	//drawBitmap(p,0,0,ALIGN_LEFT);
	CarPlayer *p=newPlayer(360,450);
	movePlayer(p);
	exitGame();
	return 0;
}
