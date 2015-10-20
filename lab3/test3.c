#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test3.h"

#define BIT(n) (0x01<<(n))

#define DELAY_US       20000
#define STAT_REG       0x64
#define KBC_CMD_REG    0x64
#define ESC_BREAK_CODE 0x81
#define OUT_BUF        0x60
#define PAR_ERR        BIT(7)
#define TO_ERR         BIT(6)
#define NOTIFICATION_ID   1
#define MSB           BIT(7)
#define OBF BIT(0)
#define IBF BIT(1)
#define KBD_IRQ 0

static int hook_id = NOTIFICATION_ID;

int kbd_test_scan(unsigned short ass) {

	unsigned long stat;
	unsigned long data;
	unsigned long data2;
	char cmd;

	if (ass == 0) {

		timer_subscribe_int();

		while (1) {
			if (sys_inb(STAT_REG, &stat) == OK)
			{
				if((stat&IBF)==0)
				{
					sys_outb(KBC_CMD_REG,cmd);
					break;
				}
				else break;
				tickdelay(micros_to_ticks(DELAY_US));
			} else
				printf("error in interruption");

		}
		do {
			if (sys_inb(STAT_REG, &stat) == OK) {
				if (stat & OBF) {
					if (sys_inb(OUT_BUF, &data)) {
						if ((stat & (PAR_ERR | TO_ERR)) == 0) {
							data2=data<<8;
							data2=data2>>8;
							if(data2==0xE0){
								data2=data>>8;
								if(data2 & MSB){
									printf("Breakcode:");
									printf("%x", data2);
								}
								else{
									printf("Makecode:");
									printf("%x",data2);
								}
							}
							else{
								if(data & MSB){
									printf("Breakcode:");
									printf("%x", data);
								}
								else{
									printf("Makecode:");
									printf("%x",data);
								}
							}

						} else
							return -1;
					} else
						printf("error in interruption");
				}
				tickdelay(micros_to_ticks(DELAY_US));
			} else
				printf("error in interruption");

		} while (data != ESC_BREAK_CODE);
		printf("oi");
		return 0;

	}
	else {
		printf("Assembly function not yet completed");
		return 1;
	}
	timer_unsubscribe_int();
	return 0;
}

int timer_subscribe_int(void) {

	if ((sys_irqsetpolicy( KBD_IRQ,(IRQ_REENABLE | IRQ_EXCLUSIVE),&hook_id) == OK)) {
		sys_irqenable(&hook_id); //ativa as interrupcoes
		return BIT(hook_id);
	} else
		return -1;

}

int timer_unsubscribe_int() {

	if (sys_irqdisable(&hook_id) == OK) //desativar as interrupcoes
	{
		sys_irqrmpolicy(&hook_id); //Remove a policy do qual foi feito o disable
		return 0;
	} else
		return 1;
}

int kbd_test_leds(unsigned short n, unsigned short *leds) {
	/* To be completed */
}

int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
