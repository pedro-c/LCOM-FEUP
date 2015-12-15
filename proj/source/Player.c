#include "Player.h"
#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <stdio.h>
#include "interface.h"
#include "keyboard.h"
#include "Obstacles.h"


#define MAKECODE_A 0x1e
#define MAKECODE_D 0x20

CarPlayer* newPlayer(int x, int y) {
	CarPlayer* p = (CarPlayer*) malloc(sizeof(CarPlayer));
	p->x = x;
	p->y = y;
	p->car=loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/car4.bmp");
	return p;
}

void setCoordinates(CarPlayer* p, int x, int y) {
	p->x = x;
	p->y = y;
}

void drawPlayer(CarPlayer* p) {
	drawBitmap(p->car,p->x,p->y,ALIGN_LEFT);
}

void movePlayer(CarPlayer* p) {
	int r, ipc_status;
	message msg;
	unsigned char codigo;
	int flag=0;
	char irq_keyboard, irq_timer;

	Obstacles *obs1=newObstacle(0,0,loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/background.bmp"));

	if ((irq_keyboard = kbd_subscribe(&hook_kbd)) == -1) {
		printf("Failed keyboard subscribe.\n");
		return;
	}
	if ((irq_timer = timer_subscribe_int()) == 1) {
		printf("Failed timer subscribe.\n");
		return;
	}
	drawPlayer(p);
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
					if (checkTrackCollision(p) == 0) {
						if (codigo == MAKECODE_A)
							p->x -= 5;
						else if (codigo == MAKECODE_D)
							p->x += 5;
					} else if (checkTrackCollision(p) == 1) {
						if (codigo == MAKECODE_D)
							p->x += 5;
					} else if (checkTrackCollision(p) == 2){
						if (codigo == MAKECODE_A)
							p->x -= 5;
					}
				} else if (msg.NOTIFY_ARG & irq_timer) {
					drawTrack(obs1);
					drawPlayer(p);
					refresh();
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

int checkTrackCollision(CarPlayer* p){
	if (p->x <= 200)
		return 1;
	if (p->x >= 540)
		return 2;
	return 0;
}
