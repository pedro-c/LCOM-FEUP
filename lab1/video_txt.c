#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem;		/* Address to which VRAM is mapped */

static unsigned scr_width;	/* Width of screen in columns */
static unsigned scr_lines;	/* Height of screen in lines */

void vt_fill(char ch, char attr) {
	int i=0;
	char *ptr;
	ptr = video_mem;
	for(i = 0; i<scr_width*scr_lines;i++,ptr++) {
		*ptr=ch;
		ptr++;
		*ptr=attr;
	}
}

void vt_blank() {

	vt_fill(0x00,0x01);

}

int vt_print_char(char ch, char attr, int r, int c) {
	int i;
	char *ptr;
	if(r<=scr_lines && c<=scr_width){
		ptr=video_mem+(c+r*scr_lines)*2;
		*ptr=ch;
		ptr++;
		*ptr=attr;
		return 0;
	}
	else
		{
		return 1;
		}
}

int vt_print_string(char *str, char attr, int r, int c) {

	char *ptr;
	if(r<=scr_lines && c<=scr_width){
				ptr=video_mem+(c+r*scr_lines)*2;
				int i;
				for(i=0; i<strlen(str);i++,ptr++)
				{
					*ptr=str[i];
					ptr++;
					*ptr=attr;
				}
				return 0;
			}
			else
				{
				return 1;
				}
}

int vt_print_int(int num, char attr, int r, int c) {
		int i;
		char *ptr;
		char n;
		n= num + '0';
		if(r<=scr_lines && c<=scr_width){
			ptr=video_mem+c+r*scr_lines;
			*ptr=n;
			ptr++;
			*ptr=attr;
			return 0;
		}
		else
			{
			return 1;
			}
}


int vt_draw_frame(int width, int height, char attr, int r, int c) {

	int lastLine=r-1+height, lastColumn=c-1+width;

	if(r<=scr_lines && c<=scr_width){
		return 0;
	}

		if (width>1 && height>1)
		{
			vt_print_char(0xc9, attr, r, c);
			vt_print_char(0xbb, attr, r, lastColumn);
			vt_print_char(0xc8, attr, lastLine, c);
			vt_print_char(0xbc, attr, lastLine, lastColumn);
		}
		else if (height==1)
				vt_print_char(0xcd, attr, r, c);
		else if (width==1)
				vt_print_char(0xba, attr, r, c);

		if (width>1)
		{
			int i;
			for (i=1;i<=width-2;i++)
				{
					vt_print_char(0xcd, attr, r , c+i);
					vt_print_char(0xcd, attr, lastLine , c+i);
				}
		}

		if(height>1)
		{
			int i;
			for (i=1; i<=height-2;i++)
			{
				vt_print_char(0xba, attr, r+i , c);
				vt_print_char(0xba, attr, r+i , lastColumn);
			}

		}

		return 0;
}


/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

  int r;
  struct mem_range mr;

  /* Allow memory mapping */

  mr.mr_base = (phys_bytes)(vi_p->vram_base);
  mr.mr_limit = mr.mr_base + vi_p->vram_size;

  if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
	  panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

  /* Map memory */

  video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vi_p->vram_size);

  if(video_mem == MAP_FAILED)
	  panic("video_txt couldn't map video memory");

  /* Save text mode resolution */

  scr_lines = vi_p->scr_lines;
  scr_width = vi_p->scr_width;

  return video_mem;
}
