#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"
#include "i8254.h"

	int main() {
		sef_startup();
		unsigned char x;
		timer_get_conf(0x00,&x);
		printf("%x\n",x);
		timer_set_square(0,60);
		return 0;
	}
