#include "GameState.h"

#include "stdio.h"
#include "score.h"

#define MAKECODE_A 0x1e
#define MAKECODE_D 0x20
#define OBS_LEFT_X   200
#define OBS_CENTER_X 340
#define OBS_RIGHT_X  480

GameState* newGameState(){
	srand(time(NULL));
	GameState* g = (GameState*) malloc(sizeof(GameState));

	g->score=loadScoreFont();;
	g->y=0;
	g->complete=0;
	g->background=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/gamebackground.bmp");
	g->player=newPlayer();
	int i, dist=0;
	for(i=0;i<OBS_SIZE;i++)
	{
		g->obs_l[i] = newObstacle(OBS_LEFT_X, dist);
		g->obs_c[i] = newObstacle(OBS_CENTER_X, dist);
		g->obs_r[i] = newObstacle(OBS_RIGHT_X, dist);
		dist+=200;
	}


	return g;
}

int updateGameState(GameState* g, unsigned long scancode, int counter) {
	if (scancode == VAL_ESC) {
		g->complete = 1;
		return 1;
	}

	moveBackGround(g);
	movePlayer(g->player, scancode);
	updateCounter(g->player);
	if (counter > 120) {
		unsigned i;
		for (i = 0; i < OBS_SIZE; i++) {
			while (1) {
				if (g->obs_l[i]->use + g->obs_c[i]->use + g->obs_r[i]->use == 2)
					break;
				updateObstacleUse(g->obs_l[i]);
				updateObstacleUse(g->obs_c[i]);
				updateObstacleUse(g->obs_r[i]);
			}
			if (g->obs_l[i]->use == 1)
				updateObstacle(g->obs_l[i]);
			else
				g->obs_l[i]->y = 0;

			if (g->obs_c[i]->use == 1)
				updateObstacle(g->obs_c[i]);
			else
				g->obs_c[i]->y = 0;

			if (g->obs_r[i]->use == 1)
				updateObstacle(g->obs_r[i]);
			else
				g->obs_r[i]->y = 0;
		}
		if (checkEndGame(g) == 1) {
			//*keyCode=VAL_ESC;
			return 1;
		}
	}

	return 0;

}

void drawGameState(GameState* g,int counter){
	int score;
	drawBitmap(g->background,0,g->y,ALIGN_LEFT);
	drawPlayer(g->player);
	scoreDisplay(g->score, counter);
if(counter>120){
	unsigned i;
	for(i=0;i<OBS_SIZE;i++)
	{
		if (g->obs_l[i]->use == 1)
			drawObstacle(g->obs_l[i]);
		if (g->obs_c[i]->use == 1)
			drawObstacle(g->obs_c[i]);
		if (g->obs_r[i]->use == 1)
			drawObstacle(g->obs_r[i]);
	}
}

}

void deleteGameState(GameState* g){
	if(g==NULL)
		return;
	deleteBitmap(g->background);
	deletePlayer(g->player);
	unsigned i;
	for(i=0;i<OBS_SIZE;i++)
	{
		deleteObstacle(g->obs_l[i]);
		deleteObstacle(g->obs_c[i]);
		deleteObstacle(g->obs_r[i]);
	}
	free(g);
}

void moveBackGround(GameState* g){
	g->y+=3;
	if(g->y>=getVerResolution())
		g->y=0;
}

int checkEndGame(GameState *g){
	int final = 0;
	unsigned i;
	Collisions* player;
	Collisions* col_l;
	Collisions* col_c;
	Collisions* col_r;
	for(i=0;i<OBS_SIZE;i++)
	{
		player = newCollision(g->player->x, g->player->y,
				g->player->x + g->player->wPlayer,
				g->player->y + g->player->hPlayer);
		col_l = newCollision(g->obs_l[i]->x, g->obs_l[i]->y,
				g->obs_l[i]->x + g->obs_l[i]->wh,
				g->obs_l[i]->y + g->obs_l[i]->ht);
		col_c = newCollision(g->obs_c[i]->x, g->obs_c[i]->y,
				g->obs_c[i]->x + g->obs_c[i]->wh,
				g->obs_c[i]->y + g->obs_c[i]->ht);
		col_r = newCollision(g->obs_r[i]->x, g->obs_r[i]->y,
				g->obs_r[i]->x + g->obs_r[i]->wh,
				g->obs_r[i]->y + g->obs_r[i]->ht);

		if (checkCollision(player, col_l) == 1)
			final = 1;
		if (checkCollision(player, col_c) == 1)
			final = 1;
		if (checkCollision(player, col_r) == 1)
			final = 1;
	}

	deleteCollision(player);
	deleteCollision(col_l);
	deleteCollision(col_c);
	deleteCollision(col_r);

	return final;
}

