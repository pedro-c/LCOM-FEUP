#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "keyboard.h"
#include "video_gr.h"

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


int wait_for_ESC() {
	int ipc_status, r;
	unsigned long irq_set;
	unsigned char codigo;
	message msg;

	if ((irq_set = kbd_subscribe(&hook_kbd)) == -1)
		return 1;

	while(codigo!=VAL_ESC){
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					kbd_code_scan(&codigo);
				}
				break;
			default:
				break;
			}
		}
	}

	if (vg_exit() != 0) {
		printf("\texit_on_ESC(): vg_exit() failed\n");
		return 1;
	}

	if(kbd_unsubscribe(&hook_kbd)!=0)
	{
		printf("Failed unsubscribe.\n");
		return 1;
	}
	return 0;
}

