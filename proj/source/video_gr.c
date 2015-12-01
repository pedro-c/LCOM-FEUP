#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "sprite.h"
#include "video_gr.h"
#include "vbe.h"
#include "timer.h"
#include "keyboard.h"
#include "i8254.h"

static int hook_id = NOTIFICATION_TIMER;
static unsigned long counter = 0;
#define DELAY_US 20000

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

static char *video_mem; /* Process address to which VRAM is mapped */

static unsigned h_res; /* Horizontal screen resolution in pixels */
static unsigned v_res; /* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */


int vg_exit() {
	struct reg86u reg86;

	reg86.u.b.intno = 0x10; /* BIOS video services */

	reg86.u.b.ah = 0x00; /* Set Video Mode function */
	reg86.u.b.al = 0x03; /* 80x25 text mode*/

	if (sys_int86(&reg86) != OK) {
		printf("\tvg_exit(): sys_int86() failed \n");
		return 1;
	} else
		return 0;
}

void *vg_init(unsigned short mode) {
	int r;
	struct mem_range mr;
	unsigned long vram_size;
	struct reg86u reg;
	vbe_mode_info_t vm;

	reg.u.w.ax = SET_VBE_MODE;
	reg.u.w.bx = 1 << 14 | mode;
	reg.u.b.intno = VIDEO_CARD;
	if (sys_int86(&reg) != OK) {
		printf("Failed sys_int86().\n");
	}

	if (vbe_get_mode_info(mode, &vm) == 1) {
		printf("Failed vbe_get_mode_info().\n");
		return;
	}

	h_res = vm.XResolution;
	v_res = vm.YResolution;
	bits_per_pixel = vm.BitsPerPixel;




	mr.mr_base = (phys_bytes)(vm.PhysBasePtr);
	mr.mr_limit = mr.mr_base + (h_res * v_res * bits_per_pixel) / 8;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_gr: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, h_res * v_res * bits_per_pixel/8);

	if (video_mem == MAP_FAILED)
		panic("Couldn't map video memory");
	return video_mem;

}

void fill_pixel(unsigned short x, unsigned short y, unsigned long color) {
	char *ptr = video_mem;
	ptr += h_res * y + x;
	*ptr = color;
}

void clear_pixels() {

	char *ptr = video_mem;
	int pixels = h_res * v_res;
	int i = 0;
	for (i; i < pixels; i++) {
		ptr += i;
		*ptr = 0;
	}

}

int print_square(unsigned short x, unsigned short y, unsigned short size,
		unsigned long color) {
	if ((x + size) > h_res || (y + size) > v_res || x < 0 || y < 0 || size <= 0)
		return 1;
	unsigned int xi = x, yi = y;
	for (y; y < (yi + size); y++) {
		x = xi;
		for (x; x < (xi + size); x++) {
			fill_pixel(y, x, color);
		}
	}
	return 0;
}

int print_line(unsigned short xi, unsigned short yi, unsigned short xf,	unsigned short yf, unsigned long color) {

	/*
	 //Bresenham's algorithm
	 int dx = (xf - xi);
	 int dy = (yf - yi);
	 int x = xi;
	 int y = yi;
	 int p = 2 * (dy) - (dx);
	 int i = 1;

	 if (xf > h_res || yf > v_res || xi < 0 || yi < 0)
	 return 1;

	 //Bresenham's algorithm

	 do {
	 fill_pixel(x, y, color);
	 while (p >= 0) {
	 y = y + 1;
	 p = p - (2 * dx);
	 }
	 x = x + 1;
	 p = p + (2 * dy);
	 i = i + 1;
	 } while (i <= dx);


	 while(x<xf){
	 fill_pixel(x, y, color);
	 p+=2*dy;
	 if(p>0){
	 y++;
	 p=p-(2*dx);
	 }
	 x++;
	 }


	 return 0;
	 */

	int d,dx,dy, temp, Eincr, NEincr;

	if(xf>H_RES || xf<0 || yf >V_RES || yf<0 || xi>H_RES || xi<0 || yi>V_RES || yi<0)
		return 1;

	dx=abs(xf-xi);
	dy=abs(yf-yi);

	if(dy <=dx){

		if (xf < xi) {
			temp = xf;
			xf = xi;
			xi = temp;
			temp = yf;
			yf = yi;
			yi = temp;
		}
		d=2*dy-dx;
		Eincr = 2*dy;
		NEincr = 2*(dy - dx);
		fill_pixel(xi,yi,color);
		if (yf > yi){
			for (xi++; xi <= xf; xi++)
			{
				if (d < 0)
					d += Eincr;
				else
				{
					d += NEincr;
					yi++;
				}
				fill_pixel(xi,yi,color);
			}
		}
		else{
			for (xi++; xi <= xf; xi++)
			{
				if (d < 0)
					d += Eincr;
				else
				{
					d += NEincr;
					yi--;
				}
				fill_pixel(xi,yi,color);
			}
		}
	}
	else {
		if (yf < yi) {
			temp = xf;
			xf = xi;
			xi = temp;
			temp = yf;
			yf = yi;
			yi = temp;
		}
		d = 2 * dx - dy;
		Eincr = 2 * dx;
		NEincr = 2 * (dx - dy);
		fill_pixel(xi,yi,color);

		if (xf > xi) {
			for (yi++; yi <= yf; yi++) {
				if (d < 0)
					d += Eincr; /* Choose the Eastern Pixel */
				else {
					d += NEincr; /* Choose the North Eastern Pixel */
					xi++; /* (or SE pixel for dx/dy < 0!) */
				}
				fill_pixel(xi,yi,color); /* Draw the point */
			}
		} else {
			for (yi++; yi <= yf; yi++) {
				if (d < 0)
					d += Eincr; /* Choose the Eastern Pixel */
				else {
					d += NEincr; /* Choose the North Eastern Pixel */
					xi--; /* (or SE pixel for dx/dy < 0!) */
				}
				fill_pixel(xi,yi,color); /* Draw the point */
			}
		}

	}

	return 0;
}

char *read_xpm(char *map[], int *wd, int *ht) {
	{
	  int width, height, colors;
	  char sym[256];
	  int  col;
	  int i, j;
	  char *pix, *pixtmp, *tmp, *line;
	  char symbol;

	  /* read width, height, colors */
	  if (sscanf(map[0],"%d %d %d", &width, &height, &colors) != 3) {
	    printf("read_xpm: incorrect width, height, colors\n");
	    return NULL;
	  }
	#ifdef DEBUG
	  printf("%d %d %d\n", width, height, colors);
	#endif
	  if (width > H_RES || height > V_RES || colors > 256) {
	    printf("read_xpm: incorrect width, height, colors\n");
	    return NULL;
	  }

	  *wd = width;
	  *ht = height;

	  for (i=0; i<256; i++)
	    sym[i] = 0;

	  /* read symbols <-> colors */
	  for (i=0; i<colors; i++) {
	    if (sscanf(map[i+1], "%c %d", &symbol, &col) != 2) {
	      printf("read_xpm: incorrect symbol, color at line %d\n", i+1);
	      return NULL;
	    }
	#ifdef DEBUG
	    printf("%c %d\n", symbol, col);
	#endif
	    if (col > 256) {
	      printf("read_xpm: incorrect color at line %d\n", i+1);
	      return NULL;
	    }
	    sym[col] = symbol;
	  }

	  /* allocate pixmap memory */
	  pix = pixtmp = malloc(width*height);

	  /* parse each pixmap symbol line */
	  for (i=0; i<height; i++) {
	    line = map[colors+1+i];
	#ifdef DEBUG
	    printf("\nparsing %s\n", line);
	#endif
	    for (j=0; j<width; j++) {
	      tmp = memchr(sym, line[j], 256);  /* find color of each symbol */
	      if (tmp == NULL) {
	    	  printf("read_xpm: incorrect symbol at line %d, col %d\n", colors+i+1, j);
	    	  return NULL;
	      }
	      *pixtmp++ = tmp - sym; /* pointer arithmetic! back to books :-) */
	#ifdef DEBUG
	      printf("%c:%d ", line[j], tmp-sym);
	#endif
	    }
	  }

	  return pix;
	}
}

int print_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	unsigned int x_var = xi, y_var = yi;
	char *pix;
	int widht, height, tmp = 0;
	pix = read_xpm(xpm, &widht, &height);
	if (pix == NULL) {
		printf("Failed read_xpm.\n");
		return 1;
	}

	for (y_var; y_var < (yi + height); y_var++) {
		x_var = xi;
		for (x_var; x_var < (xi + widht); x_var++) {
			fill_pixel(x_var, y_var, pix[tmp]);
			tmp++;
		}
	}
	return 0;
}

int move_xpm(unsigned short xi, unsigned short yi, char *xpm[],	unsigned short hor, short delta, unsigned short time) {
	unsigned int x = 0;
	unsigned int y = 0;
	unsigned int deltax = 0;
	unsigned int deltay = 0;
	unsigned int xf = 0;
	unsigned int yf = 0;
	unsigned char codigo;
	unsigned int tempx = 0;
	unsigned int tempy = 0;
	//variaveis do timer
	int ipc_status;
	message msg;
	int r;
	int counter=0;

	//create sprite
		Sprite *sp = create_sprite(xpm);
	//verifica se fica dentro do ecra
	if (hor == 1) {
		if ((xi + delta) <= 0 || (xi + delta+sp->width) >= h_res) {
			printf("Wrong coordinates.\n");
			return 1;
		}

	} else {
		if ((yi + delta) <= 0 || (yi + delta+sp->height) >= v_res) {
			printf("Wrong coordinates.\n");
			return 1;
		}

	}
	if (xi <= 0 || yi <= 0 || xi >= h_res || yi >= v_res) {
		printf("Wrong coordinates.\n");
		return 1;
	}


	unsigned long keyboard_set;
	unsigned long timer_set;
	keyboard_set = kbd_subscribe(&hook_kbd);
	timer_set = timer_subscribe_int();

	//calcula a velocidade
	unsigned long v1 = ((float) abs(delta) / (float) (time * 60));
	unsigned long v2 = ((float) (time * 60) / (float) abs(delta));



	if (v1 > v2) {


		if (hor == 1) {
			sp->xspeed = (int) v1;
			sp->yspeed = 0;
			deltax = delta;
			deltay = 0;
		} else {
			sp->xspeed = 0;
			sp->yspeed = (int) v1;
			deltax = 0;
			deltay = delta;
		}

		sp->x = xi;
		sp->y = yi;
		//draw_sprite(sp);

		while (1) {

			r = driver_receive(ANY, &msg, &ipc_status);
			if (r != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			//draw_sprite(sp);
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & keyboard_set) {
						kbd_code_scan(&codigo);
					}

					if (msg.NOTIFY_ARG & timer_set) {
						if (delta < 0) {
							tempx = sp->x - sp->xspeed;
							tempy = sp->y - sp->yspeed;

							if ((tempx >= (xi + deltax)) && (tempy >= (yi + deltay))) {
								wipe_sprite(sp);
								sp->x -= sp->xspeed;
								sp->y -= sp->yspeed;
								draw_sprite(sp);
							}
						} else {
							tempx = sp->x + sp->xspeed;
							tempy = sp->y + sp->yspeed;

							if ((tempx <= (xi + deltax)) && (tempy <= (yi + deltay))) {
								wipe_sprite(sp);
								sp->x += sp->xspeed;
								sp->y += sp->yspeed;
								draw_sprite(sp);
							}
						}


					}
					break;
				default:
					break;
				}
			}
			if (codigo == VAL_ESC)
				break;
		}
	}
	else {

		if (hor == 1) {
			sp->xspeed = 1;
			sp->yspeed = 0;
			deltax = delta;
			deltay = 0;
		} else {
			sp->xspeed = 0;
			sp->yspeed = 1;
			deltax = 0;
			deltay = delta;
		}

		sp->x = xi;
		sp->y = yi;
		//draw_sprite(sp);

		while (1) {

			r = driver_receive(ANY, &msg, &ipc_status);
			if (r != 0) {
				printf("driver_receive failed with: %d", r);
				continue;
			}

			//draw_sprite(sp);
			if (is_ipc_notify(ipc_status)) {
				switch (_ENDPOINT_P(msg.m_source)) {
				case HARDWARE:
					if (msg.NOTIFY_ARG & keyboard_set) {
						kbd_code_scan(&codigo);
					}

					if (msg.NOTIFY_ARG & timer_set) {
						if(counter==v2){
							if (delta < 0) {
								tempx = sp->x - sp->xspeed;
								tempy = sp->y - sp->yspeed;

								if ((tempx >= (xi + deltax)) && (tempy >= (yi + deltay))) {
									wipe_sprite(sp);
									sp->x -= sp->xspeed;
									sp->y -= sp->yspeed;
									draw_sprite(sp);
								}
							} else {
								tempx = sp->x + sp->xspeed;
								tempy = sp->y + sp->yspeed;

								if ((tempx <= (xi + deltax)) && (tempy <= (yi + deltay))) {
									wipe_sprite(sp);
									sp->x += sp->xspeed;
									sp->y += sp->yspeed;
									draw_sprite(sp);
								}
							}
							counter=0;
						}
						else
							counter++;


					}
					break;
				default:
					break;
				}
			}
			if (codigo == VAL_ESC)
				break;
		}

	}


	destroy_sprite(sp);

	if (kbd_unsubscribe(&hook_kbd) == 1) {
		return 1;
	}
	if (timer_unsubscribe_int() == 1) {
		return 1;
	}
	return 0;

}

int getHorResolution(){
	return h_res;
}

int getVerResolution(){
	return v_res;
}

char *getGraphicsBuffer(){
	return video_mem;
}
