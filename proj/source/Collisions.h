#ifndef __COLLISIONS_H
#define __COLLISIONS_H

typedef struct{
	int x1,x2,y1,y2;
} Collisions;

Collisions* newCollision(int x1,int x2,int x3,int x4);

int checkCollision(Collisions* col1,Collisions* col2); //palyer no primeiro parametro e obstaculo no segundo

void deleteCollision(Collisions* col);

#endif
