#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "i8254.h"

static int hook_kbd=NOTIFICATION_KBD;

int kbd_subscribe(int *hook_kbd);

int kbd_unsubscribe(int *hook_kbd);

int kbd_code_scan(unsigned char *codigo);

#endif
