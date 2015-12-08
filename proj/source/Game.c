#include <minix/drivers.h>
#include "keyboard.h"
#include "timer.h"
#include "Bitmap.h"
#include "Game.h"


Game* startGame(){
	Game* g =(Game*)malloc(sizeof(Game));


	//load menu
	g->menu=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/menu.bmp");

	return g;
}

void drawMenu(Game* g){
	drawBitmap(g->menu,0,0,ALIGN_LEFT);
}

