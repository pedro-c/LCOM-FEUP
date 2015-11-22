#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/syslib.h>
#include "sprite.h"
#include "video_gr.h"
#include "test5.h"



Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed) {
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL) {
		return NULL;
	}

	sp->map= read_xpm(pic, &(sp->width), &(sp->height));

	sp->x=x;
	sp->y=y;
	sp->xspeed=xspeed;
	sp->yspeed=yspeed;

	return sp;

}

void destroy_sprite(Sprite *sp) {

	if( sp == NULL ) {
		return;
	}
	free(sp->map);
	free(sp);

}

void draw_sprite(Sprite *sp, int xi, int yi, int delta, int x, int y){

	if(x==0 && y==0){
		continue;
	}
	else{
		sp->x=x;
		sp->y=y;
	}

	int i, j;
	if(((sp->x<(xi+delta)) && (sp->y<(yi+delta))){
		for(i = sp->x; i<(sp->x+sp->widht);i++){
			for(j = sp->y; j < (sp->y + sp->height); j++ ){
				if(*pixmap !=0){
					fill_pixel(i, j, *pixmap);
				}
				pixmap++;
			}
		}
	}

}
