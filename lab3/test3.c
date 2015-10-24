#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test3.h"
#include "keyboard.h"
#include "i8254.h"

int kbd_test_scan(unsigned short ass) {
	int r,ipc_status;
	message msg;
	int hook_kbd = NOTIFICATION_KBD;
	unsigned char codigo;
	char irq_set = BIT(hook_kbd);
	int verifica=0; //verificar se é uma tecla com info de apenas um byte ou dois


	if (ass == 0) { //realizar a funcao em IH
		if (kbd_subscribe(&hook_kbd) == -1) {
			printf("Unable to subscribe!\n");
			return -1;
		}
		while (codigo != VAL_ESC) { //enquanto nao for pressionado o ESC continua
			if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & irq_set) {
						if (verifica == 0) {
							if (kbd_code_scan(&codigo) == 0) {
								print_code(codigo);
							} else {verifica = 1;
								}
						} else {
							if (kbd_code_scan(&codigo) == 0) {
								print_code(codigo);
							}else{
							verifica = 0;
							}
						}
					}
					break;
				default:
					break;
				}
			}
		}
		printf("Pressionou a tecla ESC!\n");
		if (kbd_unsubscribe(&hook_kbd) == -1)
			printf("Can't unsubscribe!\n");
		return -1;
	}
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	/* To be completed */
}

int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
