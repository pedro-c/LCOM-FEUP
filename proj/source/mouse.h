#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/drivers.h>
#include <stdbool.h>
#include <math.h>
#include "i8254.h"

long packet[3];
static int hook_mouse=NOTIFICATION_MOUSE;
static int hook_timer=NOTIFICATION_TIMER;

/*
typedef struct {
    int x, y;
    int xSign, ySign;
    int deltaX, deltaY;
    double speedMultiplier;

    int byteBeingRead;
    unsigned long packet[3];

    int leftButtonDown;
    int middleButtonDown;
    int rightButtonDown;

    int leftButtonReleased;
    int rightButtonReleased;
    int middleButtonReleased;

    int size;
    int color1, color2;

    int hasBeenUpdated;
    int draw;
} Mouse;
*/

int mouse_subscribe();
int mouse_unsubscribe();
int mouse_write_command(char port, unsigned char cmd);
int mouse_read();
int get_packet();
void mouse_print();
//int mouseInside(int x1, int y1, int x2, int y2);
//int mouseInsideRect(Rectangle* rect);

#endif
