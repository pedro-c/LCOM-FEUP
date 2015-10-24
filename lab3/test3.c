#include <minix/sysutil.h>
#include <minix/syslib.h>
#include <minix/drivers.h>
#include "test3.h"
#include "keyboard.h"
#include "i8254.h"

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

int kbd_test_leds(unsigned short n, unsigned short *leds) {

	//0 - scrlock
	//1 - numlock
	//2 - capslock

	unsigned long irq_set;
	int i=0;
	int it = n*60; //numero de iterações vezes o tempo (60 segundos) de cada iteração
	int r;
	int ipc_status;
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
				if(msg.NOTIFY_ARG & irq_set)

			}
		}



	}



}

int kbd_test_timed_scan(unsigned short n) {
	/* To be completed */
}
