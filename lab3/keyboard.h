#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#define OUT_BUF 0x60
#define KBD_IRQ 0x01
#define VAL_ESC 0x81

static int hook_kbd=1;

int kbd_subscribe(int *hook_kbd);

int kbd_unsubscribe(int *hook_kbd);

int kbd_code_scan (unsigned char *codigo);

void print_code(unsigned char codigo);






#endif
