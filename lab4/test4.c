#include "test4.h"
#include "mouse.h"

int test_packet(unsigned short cnt) {
	int conta = 0, ind = 0, r, ipc_status;
	unsigned long irq_set;
	char cmd, packet[3];
	message msg;
	if ((irq_set = mouse_subscribe()) == -1) {
		printf("Unable to subscribe mouse!\n");
		return 1;
	}
	if (sys_outb(STAT_REG, 0xA8) != OK)
		printf("Error\n");
	if (sys_outb(STAT_REG, MC) != OK)
		printf("Error-MC\n");
	if (sys_outb(OUT_BUF, ENABLE_SEND) != OK)
		printf("Error-SEND\n"); // Enable Sending Data Packets
	while (cnt > conta) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					cmd = mouse_read();
					switch (ind) {
					case 0:
						if (cmd & BIT(3))
							packet[0] = cmd;
						break;
					case 1:
						packet[1] = cmd;
						break;
					case 2:
						packet[2] = cmd;
						conta++;
						mouse_print(packet);
						break;
					}
					if (ind == 2)
						ind = 0;
					else
						ind++;
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	printf("Terminou!\n");
	if (sys_outb(STAT_REG, MC) != OK)
		printf("ERROR-MC");
	if (sys_outb(OUT_BUF, DISABLE_STREAM) != OK)
		printf("ERROR-DISABLE_STREAM");
	if (mouse_unsubscribe() == -1)
		printf("falhou unsubscribe mouse!\n");
	return 0;
}

int test_async(unsigned short idle_time) {
	int conta = 0, ind = 0, r, ipc_status, conta_timer = 0, fim = 0;
	unsigned long irq_set_mouse, irq_set_timer;
	char cmd, packet[3];
	int tmp=60*idle_time;
	message msg;
	irq_set_timer=BIT(hook_timer);
	if (timer_subscribe_int() == -1) {
		printf("Unable to subscribe timer!\n");
		return 1;
	}
	if ((irq_set_mouse = mouse_subscribe()) == -1) {
		printf("Unable to subscribe mouse!\n");
		return 1;
	}
	if (sys_outb(STAT_REG, 0xA8) != OK)
		printf("Error\n");
	if (sys_outb(STAT_REG, MC) != OK)
		printf("Error-MC\n");
	if (sys_outb(OUT_BUF, ENABLE_SEND) != OK)
		printf("Error-SEND\n"); // Enable Sending Data Packets
	while (fim == 0) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set_mouse) {
					conta_timer = 0;
					cmd = mouse_read();
					switch (ind) {
					case 0:
						if (cmd & BIT(3))
							packet[0] = cmd;
						break;
					case 1:
						packet[1] = cmd;
						break;
					case 2:
						packet[2] = cmd;
						conta++;
						mouse_print(packet);
						break;
					}
					if (ind == 2)
						ind = 0;
					else
						ind++;
				}
				if (msg.NOTIFY_ARG & irq_set_timer) {
					conta_timer++;
					if (conta_timer >= tmp)
						return 0;
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	printf("Terminou!\n");
	if (sys_outb(STAT_REG, MC) != OK)
		printf("ERROR-MC");
	if (sys_outb(OUT_BUF, DISABLE_STREAM) != OK)
		printf("ERROR-DISABLE_STREAM");
	if (mouse_unsubscribe() == -1)
		printf("Error unsubscribing mouse!\n");
	return 0;
}

int test_config(void) {

	unsigned long data;
	char packet[3];
	int counter = 0;
	int ipc_status;
	message msg;
	unsigned long irq_set = mouse_subscribe();
	if (irq_set == -1) {
		printf("Error Subscribing mouse");
		return 0;
	}

	//Ativar o rato
	if (sys_outb(STAT_REG, ENABLE_M) != OK) {
		printf("Error enabling mourse.");
		return 0;
	}
	sys_inb(OUT_BUF,&data);
	if (sys_outb(STAT_REG, W_TO_MOUSE) != OK) {
		printf("Error writing byte to mouse.");
		return 0;
	}
	sys_inb(OUT_BUF,&data);
	if (sys_outb(IN_BUF, DISABLE_STREAM) != OK) {
		printf("Error disabling stream mode.");
		return 0;
	}
	sys_inb(OUT_BUF,&data);
	if (sys_outb(STAT_REG, W_TO_MOUSE) != OK) {
		printf("Error writing byte to mouse.");
		return 0;
	}
	sys_inb(OUT_BUF,&data);
	if (sys_outb(IN_BUF, STATUS_REQUEST) != OK) //GET MOUSE CONFIGURATION
			{
		printf("Error writing byte to mouse.");
		return 0;
	}
	sys_inb(OUT_BUF,&data);


	while (counter < 3) {
		int b = driver_receive(ANY, &msg, &ipc_status);

		if (b != 0) {
			printf("driver_receive failed with: %d", b);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & irq_set) {
					//unsigned long p = (unsigned long) packet[counter];
					sys_inb(OUT_BUF, &packet[counter]);
					counter++;
				}
				break;
			default:
				break;
			}
		}
	}

	if ((packet[0] & BIT(6))==BIT(6)) {
		printf("Remote (poled) mode\n");
	} else {
		printf("Stream mode\n");
	}
	if ((packet[0] & BIT(5))==BIT(5)) {
		printf("Data reporting enabled\n");
	} else {
		printf("Data reporting disabled\n");
	}
	if ((packet[0] & BIT(4))==BIT(4)) {
		printf("Scaling is 2:1\n");
	} else {
		printf("Scaling is 1:1\n");
	}
	if ((packet[0] & BIT(2))==BIT(2)) {
		printf("Middle button pressed\n");
	} else {
		printf("Middle button released\n");
	}
	if ((packet[0] & BIT(1))==BIT(1)) {
		printf("Right button pressed\n");
	} else {
		printf("Right button released\n");
	}
	if ((packet[0] & BIT(0))==BIT(0)) {
		printf("Left button pressed\n");
	} else {
		printf("Left button released\n");
	}

	int x;
	if(packet[1]==0)
		x=0;
	if(packet[1]==1)
		x=2;
	if(packet[1]==2)
		x=4;
	if(packet[1]==3)
		x=8;

	printf("Resolution : %d\n", x);
	printf("Sample Rate : %d\n", packet[2]);

	mouse_unsubscribe();
	return 0;
}

int test_gesture(short length, unsigned short tolerance) {

	/*
	typedef enum {
		INIT, DRAW, COMP
	} state_t;

	typedef enum {
		RDOW, RUP, MOVE
	} ev_type_t;

	void check_hor_line(event_t *evt) {
		static state_t st = INIT; // initial state; keep state
		switch (st) {
		case INIT:
			if (evt->type == RDOWN)
				state = DRAW;
			break;
		case DRAW:
			if (evt->type == MOVE) {
			[...] // need to check if events VERT_LINE or HOR_TOLERANCE occur
		} else if (evt->type == RUP)
			state = INIT;
		break;
	default:
		break;
		}
	}
	*/

}
