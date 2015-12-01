#include <minix/drivers.h>
#include <machine/int86.h>
#include <minix/syslib.h>
#include "sprite.h"
#include "video_gr.h"

/** Creates a new sprite with pixmap "pic", random speeds
 * (not zero) and position (within the screen limits), and
 * draws it in memory whose address is "base";
 * Returns NULL on invalid pixmap.
 */
Sprite *create_sprite(char *pic[]) {
	//allocate space for the "object"
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL)
		return NULL;
	// read the sprite pixmap
	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if (sp->map == NULL) {
		free(sp);
		return NULL;
	}

	return sp;
}

void destroy_sprite(Sprite *sp) {
	if (sp == NULL)
		return;
	free(sp->map);
	free(sp);
	sp = NULL; // hopeless: pointer is passed by value
}

/*
 int animate_sprite(Sprite *sp, char *base) {
 ...
 }
 */
/* Some useful non-visible functions */
/*
 static int check_collision(Sprite *sp, char *base) {
 ...
 }
 */

void draw_sprite(Sprite *sp) {
	char *pixmap = sp->map;
	int i, j;

	for (i = sp->y; i < (sp->y + sp->height); i++) {
		for (j = sp->x; j < (sp->x + sp->width); j++) {
			if (*pixmap != 0) {
				fill_pixel(j, i, *pixmap);
			}
			pixmap++;
		}
	}
}

void wipe_sprite(Sprite *sp) {

	int i, j;
	char *pixmap = sp->map;

	// Erase Sprite
	for (i = sp->y; i < (sp->y + sp->height); i++) {
		for (j = sp->x; j < (sp->x + sp->width); j++) {
			if (*pixmap != 0) {
				fill_pixel(j, i, 0);
			}
			pixmap++;
		}
	}

}

/*
Sprite *create_sprite(char *pic[], int x, int y, int xspeed, int yspeed) {
	Sprite *sp = (Sprite *) malloc(sizeof(Sprite));
	if (sp == NULL) {
		return NULL;
	}

	sp->map = read_xpm(pic, &(sp->width), &(sp->height));
	if ( sp->map == NULL ) {
			free(sp);
			return NULL;
		}

	sp->x = x;
	sp->y = y;
	sp->xspeed = xspeed;
	sp->yspeed = yspeed;

	return sp;

}

void destroy_sprite(Sprite *sp) {

	if (sp == NULL) {
		return;
	}
	free(sp->map);
	free(sp);

}

void draw_sprite(Sprite *sp) {

	char *pixmap = sp->map;
	int i, j;

	for (i = sp->y; i < (sp->y + sp->height); i++) {
		for (j = sp->x; j < (sp->x + sp->width); j++) {
			if (*pixmap != 0) {
				fill_pixel(j, i, *pixmap);
			}
			pixmap++;
		}
	}

}


 */
