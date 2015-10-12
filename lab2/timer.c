#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"
#include "i8254.h"
 int hook_id=1;
 unsigned long counter;


int timer_set_square(unsigned long timer, unsigned long freq) {
	unsigned long lsb;
	unsigned char msb;
	unsigned long fr;
	unsigned long st;

	fr = TIMER_FREQ / freq;
	if (timer == 0 || timer == 1 || timer == 2) {
		timer_get_conf(timer, &st);
		st = st << 7;
		st = st >> 7;
		st = (TIMER_0 + timer | TIMER_LSB_MSB | TIMER_SQR_WAVE | st); //controlador criado mantendo o counting mode
		if (sys_outb(TIMER_CTRL, st)) {
			printf("Error by calling sys_outb for confs\n");
			return 1;
		}

		lsb = (char)fr; //isola o primeiro byte da frequencia
		fr = fr>>8;
		msb = (char)fr; //isola o segundo byte da frequencia

		if (sys_outb(TIMER_0 + timer, lsb))	//envia o LSB para o timer
				{
			printf("Error by calling sys_outb for lsb.\n");
			return 1;
				}
		else if (sys_outb(TIMER_0 + timer, msb)) //envia o MSB para o timer
				{
			printf("Error by calling sys_outb for msb.\n");
			return 1;
				}
		else {
			return 0;
		}
	} else {
		printf("Not a possible timer.\n");
		return 1;
	}
}

int timer_subscribe_int(void) {

	int hook=hook_id;

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_ENABLE, &hook_id)==OK) //If the request was successfully handled
			{
				//sys_irqenable(&hook id); the policy defined in the call of sys_setpolicy already enables the IRQ?
				return BIT(hook_temp);
			}
	else return -1;

}

int timer_unsubscribe_int() {

	if(sys_irqrmpolicy(&hook_id)==OK) //unsubscribes a previous subscription of the interrupt notification associated with the specified hook_id
	{
		sys_irqdisable(&hook_id);  //disables interrupts on the IRQ line associated with the specified hook_id
		return 0;
	}
	else return 1;
}

void timer_int_handler() {
	counter++;
}

int timer_get_conf(unsigned long timer, unsigned char *st) {
	char CMD = TIMER_RB_CMD | TIMER_RB_SEL(timer) | TIMER_RB_COUNT_;
	if (sys_outb(TIMER_CTRL, CMD) == 0) {
		if (sys_inb(TIMER_0 + timer, st) == 0) {
			return 0;
		} else
			return 1;
	} else
		return 1;
}

int timer_display_conf(unsigned char conf) {
	int CM = conf & BIT(0);
	if (CM == 0) {
		printf("BCD\n");
	} else if (CM == 1) {
		printf("Binar0y\n");
	}
	int OM = conf >> 1 & (BIT(0) | BIT(1) | BIT(2));
	if (OM == 0) {
		printf("Interrupt on Terminal Count\n");
	} else if (OM == 1) {
		printf("Hardware Retriggerable One-shot\n");
	} else if (OM == 2) {
		printf("Rate Gnerator\n");
	} else if (OM == 3) {
		printf("Square Wave Mode\n");
	} else if (OM == 4) {
		printf("Software Triggered Strobe\n");
	} else if (OM == 5) {
		printf("Hardware Triggered Strobe(Retriggerable\n");
	}
	int TA = conf >> 4 & (BIT(0) | BIT(1));
	if (TA == 1) {
		printf("LSB");
	} else if (TA == 2) {
		printf("MSB\n");
	} else if (TA == 3) {
		printf("LSB followed by MSB\n");
	}
	int SC = conf >> 6 & BIT(0);
	if (SC == 0) {
		printf("Null Count=True\n");
	} else if (SC == 1) {
		printf("Null Count=False\n");
	}
	int SC1 = conf >> 7 & BIT(0);
	if (SC1 == 0) {
		printf("Output=0\n");
	} else if (SC == 1) {
		printf("Output=1\n");
	}
	return 0;
}


int timer_test_int(unsigned long time) {

}



int timer_test_square(unsigned long freq) {
	if(timer_set_square(0,freq)==0)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}


int timer_test_config(unsigned long timer) {

	unsigned char conf;
	if(timer_get_conf(timer, &conf)==0)
	{
		timer_display_conf(conf);
		return 0;
	}
	else
	{
		return 1;
	}
}

