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
		printf("falhou unsubscribe mouse!\n");
	return 0;
}

int test_config(void) {
	/* To be completed ... */
}

int test_gesture(short length, unsigned short tolerance) {
	/* To be completed ... */
}
