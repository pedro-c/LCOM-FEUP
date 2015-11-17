#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/syslib.h>

#include "test5.h"
#include "video_gr.h"
#include "timer.h"

#define TEST_SQUARE_MODE      0x105

void *test_init(unsigned short mode, unsigned short delay){

	char *video_mem;
	video_mem=vg_init(mode);

	timer_test_int(delay);

	if(vg_exit()==1)
		printf("Failed vg_exit().\n");
}

int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color){

	if(vg_init(TEST_SQUARE_MODE)==NULL)
	{
		printf("Failed vg_init().\n");
		return 1;
	}

	print_square(x,y,size,color);
	timer_test_int(10);

	if(vg_exit()!=0)
	{
		printf("Failed vg_exit.\n");
		return 1;
	}
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color){
	return 0;
}


int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]){
	return 0;
}

int test_move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time){
	return 0;
}

int test_controller(){
	return 0;
}
