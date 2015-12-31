#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "i8254.h"

static int hook_kbd=NOTIFICATION_KBD;

int kbd_subscribe();

int kbd_unsubscribe();

int kbd_code_scan();

unsigned char kbd_code();

#endif
