#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "vbe.h"

/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xF0000000
#define H_RES                 1024
#define V_RES		           768
#define BITS_PER_PIXEL	         8
#define SET_VBE_MODE        0x4F02
#define VIDEO_CARD            0x10

/* Private global variables */

static char *video_mem;		/* Process address to which VRAM is mapped */

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

void *vg_init(unsigned short mode){
	int r;
	struct mem_range mr;
	unsigned long vram_size;
	struct reg86u reg;
	vbe_mode_info_t vm;

	reg.u.w.ax=SET_VBE_MODE;
	reg.u.w.bx=1<<14|mode;
	reg.u.b.intno=VIDEO_CARD;
	if(sys_int86(&reg)!=OK)
	{
		printf("Failed sys_int86().\n");
	}

	h_res=H_RES;
	v_res=V_RES;
	bits_per_pixel=BITS_PER_PIXEL;

	mr.mr_base = VRAM_PHYS_ADDR;
	mr.mr_limit = mr.mr_base + (h_res*v_res*bits_per_pixel)/8;

	  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	  /* Map memory */

	  video_mem = vm_map_phys(SELF, (void *)mr.mr_base,(h_res*v_res*bits_per_pixel)/8);

	  if(video_mem == MAP_FAILED)
		  panic("Couldn't map video memory");
	  return video_mem;

}

void fill_pixel(unsigned short x,unsigned short y,unsigned long color){
	char *vm=video_mem;
	vm+=h_res*y;
	vm+=x;
	*vm=color;
}


int print_square(unsigned short x,unsigned short y,unsigned short size,unsigned long color)
{
	unsigned int i,j;
	for(i=y;i<(y+size);i++)
	{
		for(j=x;j<(x+size);j++)
		{
			fill_pixel(j,i,color);
		}
	}
		return 0;
}
