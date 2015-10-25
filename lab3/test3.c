#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test3.h"
#include "keyboard.h"
#include "i8254.h"

static int hook_id=NOTIFICATION_ID;

int kbd_test_scan(unsigned short ass) {
	int r,ipc_status;
	message msg;
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


int timer_subscribe_int(void) {

	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_id)==OK))
	{
		sys_irqenable(&hook_id);//ativa as interrupcoes
		return BIT(hook_id);
	}
	else return -1;

}

int timer_unsubscribe_int() {

	if(sys_irqdisable(&hook_id)==OK) //desativar as interrupcoes
	{
		sys_irqrmpolicy(&hook_id); //Remove a policy do qual foi feito o disable
		return 0;
	}
	else return 1;
}

int sendInst (unsigned long reg, unsigned long cmd)
{
	unsigned long stat, r, data, i = 0;

		while(i < 3)
		{
			if (sys_inb(STAT_REG, &stat) != OK)
			{
				printf("sys_inb failed with: %d", r);
				return -1;
			}

			if ((stat & IBF) == 0)
			{
				r = sys_outb(reg, cmd);
				if (r != OK)
				{
					printf("sys_inb failed with: %d", r);
					return -1;
				}
				else return 0;
			}

			tickdelay(micros_to_ticks(DELAY_US));

			i++;
		}

		printf("unable to write data\n");
		return -1;
}


int set_leds(unsigned long cmd){
	unsigned long data;

	sendInst(OUT_BUF, KBC_CMD_ED);
	sys_inb(OUT_BUF,&data);

	if(data==KB_ACK)
		sendInst(OUT_BUF, cmd);
	else
	{
		printf("Set command error\n");
		return -1;
	}
	return 0;
}


int kbd_test_leds(unsigned short n, unsigned short *leds) {

	//0 - scrlock
	//1 - numlock
	//2 - capslock

	unsigned long irq_set, cmd;
	int i=0;
	int it = n*60; //numero de iterações vezes o tempo (60 segundos) de cada iteração
	int r;
	int ipc_status;
	int t;
	message msg;
	unsigned long  numlk=0;
	unsigned long  scrlk=0;
	unsigned long  capslk=0;


	irq_set=timer_subscribe_int();
	if (kbd_subscribe(&hook_kbd) == -1) {
				printf("Unable to subscribe!\n");
				return -1;
	}

	while(i < it){

		if(driver_receive(ANY,&msg,&ipc_status)!=0){
			 printf("driver_receive failed with: %d", r);
			 continue;
		}

		if(is_ipc_notify(ipc_status)) //recebe notificação
		{
			switch(_ENDPOINT_P(msg.m_source)){
			case HARDWARE:
				if(msg.NOTIFY_ARG & irq_set){
					if((i % 60 ==0)||(i==0)){
						switch(leds[t])
						{
						case 0:
							if(scrlk==0) scrlk = BIT(0);
							else scrlk=0;
							break;
						case 1:
							if(numlk==0) numlk=BIT(1);
							else numlk=0;
						case 2:
							if(capslk==0) capslk=BIT(2);
							else capslk=0;
							break;
						default:
							printf("Invalid indicator");
							return 1;
							break;
						}
						cmd= scrlk|numlk|capslk;
						set_leds(cmd);
						t++;
					}
				}
				break;
			default:
				break;

			}
		}
		i++;
	}

	timer_unsubscribe_int();


}

int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
