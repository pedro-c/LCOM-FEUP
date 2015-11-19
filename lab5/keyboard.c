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

void print_code(unsigned char codigo) {
	int MSB = (0xff & codigo) >> 7;
	if (MSB == 0) {
		printf("Make code is: 0x%x\n", codigo);
	} else {
		printf("Break code is: 0x%x\n", codigo);
	}
}

int sendInst(unsigned long reg, unsigned long cmd) {
	unsigned long stat, r, data, i = 0;

	while (i < 3) {
		if (sys_inb(STAT_REG, &stat) != OK) {
			printf("sys_inb failed with: %d", r);
			return -1;
		}

		if ((stat & IBF) == 0) {
			r = sys_outb(reg, cmd);
			if (r != OK) {
				printf("sys_inb failed with: %d", r);
				return -1;
			} else
				return 0;
		}

		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}

	printf("unable to write data\n");
	return -1;
}

int set_leds(unsigned long cmd) {
	unsigned long data;

	sendInst(OUT_BUF, KBC_CMD_ED);
	sys_inb(OUT_BUF, &data);

	if (data == KB_ACK)
		sendInst(OUT_BUF, cmd);
	else {
		printf("Set command error\n");
		return -1;
	}
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

