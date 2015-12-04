#include <minix/drivers.h>
#include "timer.h"
#include "pixmap.h"
#include "Player.h"

int main(int argc, char **argv) {

	sef_startup();
	CarPlayer *p=newPlayer(10,10,pic1);
	initGame(0x114);
	movePlayer(p);
	exitGame();
	return 0;
}
