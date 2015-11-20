#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/syslib.h>

#include "test5.h"
#include "video_gr.h"
#include "timer.h"
#include "vbe.h"

#define TEST_SQUARE_MODE      0x105

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

	if (vg_init(TEST_SQUARE_MODE) == NULL) {
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

int test_line(unsigned short xi, unsigned short yi, unsigned short xf,
		unsigned short yf, unsigned long color) {
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {
	return 0;
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[],
		unsigned short hor, short delta, unsigned short time) {
	return 0;
}

int test_controller() {
	return 0;
}
