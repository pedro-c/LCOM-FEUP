#include <minix/drivers.h>
#include <stdbool.h>
#include <math.h>

#include "mouse.h"

int timer_subscribe_int(void) {
	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_timer) == OK)) {
		sys_irqenable(&hook_timer); //ativa as interrupcoes
		return BIT(hook_timer);
	} else
		return -1;
}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hook_timer) == OK) //desativar as interrupcoes
			{
		sys_irqrmpolicy(&hook_timer); //Remove a policy do qual foi feito o disable
		return 0;
	} else
		return 1;
}

int mouse_subscribe() {
	int hook_temp = hook_mouse;
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_mouse)
			== OK)
		if (sys_irqenable(&hook_mouse) == OK)
			return BIT(hook_temp);
	return -1;
}

int mouse_unsubscribe() {
	if (sys_irqdisable(&hook_mouse) == OK)
		if (sys_irqrmpolicy(&hook_mouse) == OK)
			return 0;
	return -1;
}

int mouse_write(char cnt, unsigned char cmd) {
	unsigned long stat = 0;
	unsigned int i = 0;
	while (i < 10) {
		sys_inb(STAT_REG, &stat);
		if ((stat & IBF) == 0) {
			sys_outb(cnt, cmd);
			return 0;
		}
		tickdelay(micros_to_ticks(DELAY_US));
		i++;
	}
	return 1;
}

int mouse_read() {
	unsigned long stat, data;
	unsigned int i = 0;
	while (i < 10) {
		sys_inb(STAT_REG, &stat);
		if (stat & OBF) {
			sys_inb(OUT_BUF, &data);
			if ((stat & (PAR_ERR | TO_ERR)) == 0)
				return data;
			else
				return -1;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}
}

void mouse_print(char packet[]) {
	int mb = 0, rb = 0, lb = 0, yo = 0, xo = 0, ys = 0, xs = 0;
	if (packet[0] & BIT(7))
		yo = 1;
	if (packet[0] & BIT(6))
		xo = 1;
	if (packet[0] & BIT(5))
		ys = 1;
	if (packet[0] & BIT(4))
		xs = 1;
	if (packet[0] & BIT(2))
		mb = 1;
	if (packet[0] & BIT(1))
		rb = 1;
	if (packet[0] & BIT(0))
		lb = 1;
	printf("B1: 0x%-*x ", 2, packet[0]);
	printf("B2: 0x%-*x ", 2, (packet[1] & 0xFF));
	printf("B3: 0x%-*x ", 2, (packet[2] & 0xFF));
	printf("LB: %*d ", 1, lb);
	printf("MB: %*d ", 1, mb);
	printf("RB: %*d ", 1, rb);
	printf("XOV: %*d ", 1, xo);
	printf("YOV: %*d ", 1, yo);
	if (xs == 1) {
		char p = packet[1] ^ 0xFF;
		p++;
		printf("X:%d", (short) p);
	} else
		printf("X:%d", packet[1]);
	if (ys == 1) {
		char p = packet[2] ^ 0xFF;
		p++;
		printf("Y:%d", (short) p);
	} else
		printf("Y:%d", packet[2]);
	printf("\n");
}

