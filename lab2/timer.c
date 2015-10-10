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
			int CM=conf & BIT(0);
			if(CM==0)
			{
				printf("BCD\n");
			}
			else if(CM==1)
			{
				printf("Binar0y\n");
			}
			int OM=conf >> 1 & (BIT(0)|BIT(1)|BIT(2));
			if(OM==0){
				printf("Interrupt on Terminal Count\n");
			}
			else if(OM==1){
						printf("Hardware Retriggerable One-shot\n");
					}
			else if(OM==2){
				printf("Rate Gnerator\n");
			}
			else if(OM==3){
				printf("Square Wave Mode\n");
			}
			else if(OM==4){
						printf("Software Triggered Strobe\n");
					}
			else if(OM==5){
				printf("Hardware Triggered Strobe(Retriggerable\n");
			}
			int TA=conf >> 4 & (BIT(0)|BIT(1));
			if(TA==1){
				printf("LSB");
			}
			else if(TA==2){
				printf("MSB\n");
			}
			else if(TA==3){
				printf("LSB followed by MSB\n");
			}
			int SC=conf >> 6 & BIT(0);
			if(SC==0)
			{
				printf("Null Count=True\n");
			}
			else if(SC==1){
				printf("Null Count=False\n");
			}
			int SC1=conf >> 7 & BIT(0);
			if(SC1==0)
					{
						printf("Output=0\n");
					}
					else if(SC==1){
						printf("Output=1\n");
					}
			return 0;
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
