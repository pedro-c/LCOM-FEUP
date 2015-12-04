#include "Player.h"

#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include "interface.h"
#include "keyboard.h"

#define MAKECODE_A 0x1e
#define MAKECODE_D 0x20

CarPlayer* newPlayer(int x, int y, char* pix[]) {
	CarPlayer* p = (CarPlayer*) malloc(sizeof(CarPlayer));
	p->x = x;
	p->y = y;
	p->pixm = pix;
}

void setCoordinates(CarPlayer* p, int x, int y) {
	p->x = x;
	p->y = y;
}

void drawPlayer(CarPlayer* p) {
	print_xpm(p->x, p->y, p->pixm);
}

void movePlayer(CarPlayer* p) {
	int r, ipc_status;
	message msg;
	unsigned char codigo;
	char irq_keyboard, irq_timer;
	if ((irq_keyboard = kbd_subscribe(&hook_kbd)) == -1) {
		printf("Failed keyboard subscribe.\n");
		return;
	}
	if ((irq_timer = timer_subscribe_int()) == 1) {
		printf("Failed timer subscribe.\n");
		return;
	}
	while (codigo != VAL_ESC) { //enquanto nao for pressionado o ESC continua
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_keyboard) {
					kbd_code_scan(&codigo);
					if(codigo==MAKECODE_A)
						p->x--;
					else if(codigo==MAKECODE_D)
						p->x++;
				}
				else if(msg.NOTIFY_ARG & irq_timer)
				{
					drawPlayer(p);
				}
				break;
			default:
				break;
			}
		}
	}
	if (kbd_unsubscribe(&hook_kbd) == -1) {
		printf("Failed keyboard unsubscribe.\n");
		return;
	}

	if (timer_unsubscribe_int() == 1) {
		printf("Failed timer unsubscribe.\n");
		return;
	}
}
