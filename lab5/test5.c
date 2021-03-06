#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/syslib.h>

#include "test5.h"
#include "video_gr.h"
#include "timer.h"
#include "vbe.h"
#include "sprite.h"


#define MODE_105      0x105

void *test_init(unsigned short mode, unsigned short delay) {

	char *video_mem;
	vbe_mode_info_t vbe;
	video_mem = vg_init(mode);

	if (vbe_get_mode_info(mode, &vbe) == 1) {
		printf("Failed vbe_get_mode_info().\n");
		return;
	}

	timer_test_int(delay);

	if (vg_exit() == 1) {
		printf("Failed vg_exit().\n");
		return;
	}

	printf("Physical Adress VRAM: 0x%x",vbe.PhysBasePtr);
	return video_mem;
}

int test_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {

	if (vg_init(MODE_105) == NULL) {
		printf("Failed vg_init().\n");
		return 1;
	}

	if(print_square(x, y, size, color)==1)
	{
		if(vg_exit()==1)
		{
			printf("Failed vg_exit().\n");
		}
		printf("Wrong coordinates.\n");
		return 1;
	}

	if (wait_for_ESC() ==1) {
		printf("Failed wait_for_ESC().\n");
		return 1;
	}
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {
	if(vg_init(MODE_105)==NULL)
	{
		printf("Failed vg_init().\n");
		return 1;
	}

	if(print_line(xi,yi,xf,yf,color)==1)
		{
			if(vg_exit()==1)
			{
				printf("Failed vg_exit().\n");
			}
			printf("Wrong coordinates.\n");
			return 1;
		}

	if (wait_for_ESC() ==1) {
			printf("Failed wait_for_ESC().\n");
			return 1;
		}
		return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	if(vg_init(MODE_105)==NULL)
	{
		printf("Failed vg_init().\n");
		return 1;
	}
	if(print_xpm(xi,yi,xpm)==1)
		{
			if(vg_exit()==1)
			{
				printf("Failed vg_exit().\n");
			}
			printf("Wrong coordinates.\n");
			return 1;
		}

	if (wait_for_ESC() ==1) {
			printf("Failed wait_for_ESC().\n");
			return 1;
		}
		return 0;

}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {

	if (vg_init(MODE_105) == NULL) {
		printf("Failed vg_init().\n");
		return 1;
	}

	if (move_xpm(xi, yi, xpm, hor, delta, time) == 1) {
		if (vg_exit() == 1) {
			printf("Failed vg_exit().\n");
		}
		printf("Wrong coordinates.\n");
		return 1;

	}
	if (wait_for_ESC() == 1) {
		printf("Failed wait_for_ESC().\n");
		return 1;
	}

	return 0;
}

int test_controller() {
	return 0;
}
