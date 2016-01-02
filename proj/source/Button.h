#pragma once

typedef struct{
	int x1,y1,x2,y2;
}Box;

Box* newBox(int x1, int y1, int x2, int y2);
void deleteBox(Box* box);
