#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>

#include "video_gr.h"
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

	if(vbe_get_mode_info(mode,&vm)==1)
	{
		printf("Failed vbe_get_mode_info().\n");
		return;
	}

	h_res=vm.XResolution;
	v_res=vm.YResolution;
	bits_per_pixel=vm.BitsPerPixel;

	mr.mr_base = (phys_bytes)(vm.PhysBasePtr);
	mr.mr_limit = mr.mr_base + (h_res*v_res*bits_per_pixel)/8;

	  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		  panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", r);

	  /* Map memory */

	  video_mem = vm_map_phys(SELF, (void *)mr.mr_base,(h_res*v_res*bits_per_pixel)/8);

	  if(video_mem == MAP_FAILED)
		  panic("Couldn't map video memory");
	  return video_mem;

}

void fill_pixel(unsigned short x,unsigned short y,unsigned long color){
	char *ptr=video_mem;
	ptr+=h_res*y+x;
	*ptr=color;
}

int print_square(unsigned short x,unsigned short y,unsigned short size,unsigned long color)
{
	if((x+size)>h_res ||(y+size)>v_res || x<0 || y<0 || size<=0)
		return 1;
	unsigned int xi=x,yi=y;
	for(y;y<(yi+size);y++)
	{
		x=xi;
		for(x;x<(xi+size);x++)
		{
			fill_pixel(y,x,color);
		}
	}
		return 0;
}

int print_line(unsigned short xi,unsigned short yi,unsigned short xf,unsigned short yf,unsigned long color)
{

	int dx = (xf - xi);
    int dy = (yf - yi);
	int x = xi;
	int y = yi;
	int p = 2 * (dy) - (dx);
	int i=1;

	if(xf>h_res ||yf>v_res || xi<0 || yi<0)
		return 1;

	//Bresenham's algorithm

	do {
	    fill_pixel(x, y, color);
	    while(p >= 0) {
	        y = y + 1;
	        p = p - (2 * dx);
	    }
	    x = x + 1;
	    p = p + (2 * dy);
	    i = i + 1;
	}
	while(i <= dx);

	return 0;

}

char *read_xpm(char *map[], int *wd, int *ht){
	int width,height,num_colors;
	unsigned int i,j,k;
	char symbols[256];
	int colors[256];
	char *pix_col,*pix_col_tmp,*line;

	for(i=0;i<256;i++)
		symbols[i]=0;
	for(i=0;i<256;i++)
		colors[i]=0;

	if (sscanf(map[0],"%d %d %d", &width, &height, &num_colors) != 3){
		printf("Wrong width/heigth/num_colors.\n");
		return NULL;
	}

	if(width>h_res || height>v_res || num_colors>256){
		printf("Wrong width/heigth/num_colors.\n");
		return NULL;
	}

	*wd=width;
	*ht=height;

	for(i=0;i<num_colors;i++){

		if (sscanf(map[i+1], "%c %d",&symbols[i],&colors[i]) != 2) {
					printf("Wrong symbol/color.\n");
					return NULL;
				}
				if (colors[i] > 256) {
					printf("Wrong color.\n");
					return NULL;
				}
	}

	pix_col = pix_col_tmp = malloc(height*width);

		/* parse each pixmap symbol line */
		for (i=0; i<height;i++) {
			line = map[num_colors+i+1];
			for (j=0; j<width;j++) {
				int cor;
				for(k=0;k<256;k++)
				{
					if(line[j]==symbols[k])
						cor=colors[k];
				}
				*pix_col_tmp++=cor;
			}
		}
		return pix_col;
}

int print_xpm(unsigned short xi,unsigned short yi,char *xpm[]){

	unsigned int x_var=xi,y_var=yi;
	char *pix;
	int widht,height,tmp=0;
	pix=read_xpm(xpm,&widht,&height);
	if(pix==NULL)
	{
		printf("Failed read_xpm.\n");
		return 1;
	}

	for(y_var;y_var<(yi+height);y_var++)
	{
		x_var=xi;
		for(x_var;x_var<(xi+widht);x_var++)
		{
			fill_pixel(x_var,y_var,pix[tmp]);
			tmp++;
		}
	}
	return 0;
}
