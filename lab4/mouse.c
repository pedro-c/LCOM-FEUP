#include <minix/drivers.h>
#include <stdbool.h>
#include <math.h>

#include "mouse.h"

int timer_subscribe_int(void) {
	if ((sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hook_timer) == OK)) {
		sys_irqenable(&hook_timer); //ativa as interrupcoes
		return BIT(hook_timer);
	} else
		return -1;
}

int timer_unsubscribe_int() {
	if (sys_irqdisable(&hook_timer) == OK) //desativar as interrupcoes
			{
		sys_irqrmpolicy(&hook_timer); //Remove a policy do qual foi feito o disable
		return 0;
	} else
		return 1;
}

int mouse_subscribe(int *hook_mouse) {
	if (sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, hook_mouse) == OK)
		if (sys_irqenable(hook_mouse) == OK)
			return 0;
	return -1;
}

int kbd_unsubscribe(int *hook_mouse) {
	if (sys_irqdisable(hook_mouse) == OK)
		if (sys_irqrmpolicy(hook_mouse) == OK)
			return 0;
	return 1;
}
