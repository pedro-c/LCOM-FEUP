#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/drivers.h>
#include <stdbool.h>
#include <math.h>

#include "i8042.h"

long packet[3];
static int hook_mouse=NOTIFICATION_MOUSE;
static int hook_timer=NOTIFICATION_TIMER;

int timer_subscribe_int(void);
int timer_unsubscribe_int();
int mouse_subscribe();
int mouse_unsubscribe();
int mouse_write_command(char port, unsigned char cmd);
int mouse_read();
int get_packet();
void mouse_print();

#endif
