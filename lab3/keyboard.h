#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "i8254.h"
#define OUT_BUF 		0x60
#define KBD_IRQ 		0x01
#define VAL_ESC 		0x81
#define ACK				0xFA
#define RESEND			0xFE
#define ERROR			0xFC
#define KB_ACK			0xFA
#define KBC_CMD_ED		(BIT(7) | BIT(6) | BIT(5) | BIT(3) | BIT(2) | BIT(0))
#define STAT_REG		0x64
#define DELAY_US 		20000
#define OBF 			BIT(0)
#define IBF 			BIT(1)

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
