#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"

static int hook_kbd=NOTIFICATION_KBD;
static int hook_timer=NOTIFICATION_TIMER;

int kbd_subscribe(int *hook_kbd);

int kbd_unsubscribe(int *hook_kbd);

int kbd_code_scan (unsigned char *codigo);

void print_code(unsigned char codigo);

int timer_subscribe_int(void);

int timer_unsubscribe_int();

int sendInst(unsigned long reg, unsigned long cmd);

int set_leds(unsigned long cmd);






#endif
