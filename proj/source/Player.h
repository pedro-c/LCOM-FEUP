#ifndef __PLAYER_H
#define __PLAYER_H

typedef struct{
	int x,y; //coordenadas do carro
	int verfifyCrash; //1-Carro colidiu,0- Não colidiu
	//Bitmap* b; Para depois
	char* pixm;
}CarPlayer;

CarPlayer* newPlayer(int x,int y,char* pix[]);

void setCoordinates(CarPlayer* p,int x,int y);

void drawPlayer(CarPlayer* p);

#endif
