#ifndef __MOUSE_H
#define __MOUSE_H

#include <minix/drivers.h>
#include <stdbool.h>
#include <math.h>
#include "i8254.h"
#include "Bitmap.h"
#include "Button.h"

unsigned long packet[3];
static int hook_mouse=NOTIFICATION_MOUSE;

typedef struct {
	int x,y;

	int lb_pressed;
	int mb_pressed;
	int rb_pressed;
	int lb_released;
	int mb_released;
	int rb_released;

	Bitmap* image;

} Mouse;
Mouse* getMouse();
void setMouse(Mouse* m);
Mouse* newMouse();
void updateMouse();
void drawMouse();
void enableMouse();
int mouse_subscribe();
int mouse_unsubscribe();
int mouse_write_command(char port, unsigned char cmd);
int mouse_read();
int get_packet();
void mouse_print();
int mouseInside(int x1, int y1, int x2, int y2);
int mouseInsideBox(Box* box);

#endif
