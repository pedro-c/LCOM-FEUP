#include <minix/drivers.h>
#include <stdbool.h>
#include "mouse.h"
#include "Bitmap.h"
#include "interface.h"

Mouse mousePreviousState;

unsigned long irq_set;
int ipc_status;
message msg;
int conta = 0, ind = 0, r, ipc_status;
unsigned long irq_set, data;
char cmd;
message msg;

Mouse* newMouse() {
	Mouse* mouse = (Mouse*) malloc(sizeof(Mouse));

	mouse->x = 400;
	mouse->y = 300;

	mouse->image = loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/mouse.bmp");

	if ((irq_set = mouse_subscribe()) == -1) {
		printf("Unable to subscribe mouse!\n");
	}
	if (sys_outb(STAT_REG, ENABLE_MOUSE) != OK) //rato enable
		printf("Error\n");
	if (sys_outb(STAT_REG, W_TO_MOUSE) != OK) //MOUSE
		printf("Error-MC\n");
	if (sys_outb(OUT_BUF, ENABLE_SEND) != OK) //Ativar o envio
		printf("Error-SEND\n");

	return mouse;
}


void drawMouse(Mouse* mouse){
	drawBitmap(mouse->image,mouse->x,mouse->y,ALIGN_LEFT);
}

void updateMouse(Mouse* mouse) {
	int mouseVelocity=3; //higher is slower
	int convert = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
	if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
		printf("driver_receive failed with: %d", r);
	}
	if (is_ipc_notify(ipc_status)) {
		switch (_ENDPOINT_P(msg.m_source)) {
		case HARDWARE:
			if (msg.NOTIFY_ARG & irq_set) {
				get_packet();

				int ys = 0;
				int xs = 0;

				if (packet[0] & BIT(5))
					ys = 1;
				if (packet[0] & BIT(4))
					xs = 1;

				if (packet[1] & BIT(7)) {
					char p = packet[1] ^ convert;
					p++;
					if (xs == 0) {
						mouse->x+=(int)((short) p/mouseVelocity);
					} else {
						mouse->x-=(int)((short) p/mouseVelocity);
					}

				} else if (xs == 0) {
					mouse->x+=(int)(packet[1]/mouseVelocity);
				} else {
					mouse->x-=(int)(packet[1]/mouseVelocity);
				}

				if (packet[2] & BIT(7)) {
					char p = packet[2] ^ convert;
					p++;
					if (ys == 1) {
						mouse->y+=(int)((short) p/mouseVelocity);
					} else {
						mouse->y-=(int)((short) p/mouseVelocity);
					}
				} else if (ys == 1) {
					mouse->y+=(int)(packet[2]/mouseVelocity);
				} else {
					mouse->y-=(int)(packet[2]/mouseVelocity);
				}

				if(mouse->x > (getHorResolution()-35)){
					mouse->x = (getHorResolution()-35);
				}
				else if(mouse->x < 1){
					mouse->x = 1;
				}
				if(mouse->y > (getVerResolution()-48)){
					mouse->y = (getVerResolution()-48);
				}
				else if(mouse->y < 1){
					mouse->y = 1;
				}

				drawMouse(mouse);

			}
			break;
		default:
			break;
		}
	} else {
	}

}


int mouse_subscribe() {
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_mouse)
			== OK)
		if (sys_irqenable(&hook_mouse) == OK)
			return BIT(NOTIFICATION_MOUSE);
	return -1;
}

int mouse_unsubscribe() {
	if (sys_irqdisable(&hook_mouse) == OK)
		if (sys_irqrmpolicy(&hook_mouse) == OK)
			return 0;
	return -1;
}

int mouse_write(unsigned long cnt, unsigned char cmd) {
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
	int ind = 0;

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
		i++;
	}
}



int get_packet(){

	long data;

	data=mouse_read();
	if(data & BIT(3)){
		packet[0] = data;
		data=mouse_read();
		packet[1] = data;
		data=mouse_read();
		packet[2] = data;
	}
	else{
		data=mouse_read();
		if(data & BIT(3)){
			packet[0] = data;
			data=mouse_read();
			packet[1] = data;
			data=mouse_read();
			packet[2] = data;
		}
		else{
			data=mouse_read();
			if(data & BIT(3)){
				packet[0] = data;
				data=mouse_read();
				packet[1] = data;
				data=mouse_read();
				packet[2] = data;
			}
		}
	}
}


void mouse_print() {
	int mb = 0, rb = 0, lb = 0, yo = 0, xo = 0, ys = 0, xs = 0;
	int convert = BIT(7) | BIT(6) | BIT(5) | BIT(4) | BIT(3) | BIT(2) | BIT(1) | BIT(0);
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
	printf("B1: 0x%x ", packet[0]);
	printf("B2: 0x%x ", packet[1]);
	printf("B3: 0x%x ", packet[2]);
	printf("LB: %*d ", 1, lb);
	printf("MB: %*d ", 1, mb);
	printf("RB: %*d ", 1, rb);
	printf("XOV: %*d ", 1, xo);
	printf("YOV: %*d ", 1, yo);
	printf("ys: %*d ", 1, ys);
	printf("xs: %*d ", 1, xs);


	if (packet[1] & BIT(7)) {
		char p = packet[1]^convert;
		p++;
		printf("X:%d ", (short) p);
	} else
		printf("X:%d ", packet[1]);
	if (packet[2] & BIT(7)) {
		char p = packet[2] ^ convert;
		p++;
		printf("Y:%d ", (short) p);
	} else
		printf("Y:%d ", packet[2]);
	printf("\n");
}



/*
int mouseInside(int x1, int y1, int x2, int y2){
	return x1 <= getMouse()->x && getMouse()->x <= x2 && y1 <= getMouse()->y && getMouse()->y <= y2;
}
int mouseInsideRect(Rectangle* rect){
	return mouseInside(rect->x1,rect->y1,rect->x2,rect->y2);
}
*/

