#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"

int kbd_subscribe(int *hook_kbd) {

	if (sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_kbd) == OK)
		if (sys_irqenable(hook_kbd) == OK)
			return BIT(NOTIFICATION_KBD);
	return -1;
}

int kbd_unsubscribe(int *hook_kbd) {
	if (sys_irqdisable(hook_kbd) == OK)
		if (sys_irqrmpolicy(hook_kbd) == OK)
			return 0;
	return 1;
}

int kbd_code_scan(unsigned char *codigo) {
	unsigned long cod;
	sys_inb(OUT_BUF, &cod);
	*codigo = (unsigned char) cod;
	if ((0xE0 & cod) == 0xE0)
		return 1;
	else
		return 0;
}