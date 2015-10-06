#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"
#include "i8254.h"

int timer_set_square(unsigned long timer, unsigned long freq) {

		return 1;
	}

	int timer_subscribe_int(void) {

		return 1;
	}

	int timer_unsubscribe_int() {

		return 1;
	}

	void timer_int_handler() {

	}

	int timer_get_conf(unsigned long timer, unsigned char *st) {
		char CMD = TIMER_RB_CMD | TIMER_RB_SEL(timer)|TIMER_RB_COUNT_;
		if(sys_outb(TIMER_CTRL,CMD)==0){
			if(sys_inb(TIMER_0+timer,st)==0){
				return 0;
			}
			else return 1;
		}
		else return 1;
	}

	int timer_display_conf(unsigned char conf) {

		return 1;
	}

	int timer_test_square(unsigned long freq) {

		return 1;
	}

	int timer_test_int(unsigned long time) {

		return 1;
	}

	int timer_test_config(unsigned long timer) {

		return 1;
	}
