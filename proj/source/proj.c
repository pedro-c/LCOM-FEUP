#include <minix/drivers.h>
#include "Bitmap.h"
#include "timer.h"

int main(int argc, char **argv) {

	sef_startup();
	vg_init(0x114);
	printf("chegou");
	Bitmap* b= loadBitmap("/home/lcom/lcom1516-t2g12/proj/res/images/teste.bmp");
	drawBitmap(b,0,0,ALIGN_LEFT);
	timer_test_int(3);
	vg_exit();
	return 0;
}
