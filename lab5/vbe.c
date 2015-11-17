#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>

#include "vbe.h"
#include "lmlib.h"

#define LINEAR_MODEL_BIT 14

#define PB2BASE(x) (((x) >> 4) & 0x0F000)
#define PB2OFF(x) ((x) & 0x0FFFF)
#define VIDEO_CARD                   0x10

int vbe_get_mode_info(unsigned short mode, vbe_mode_info_t *vmi_p) {
	return 0;
}


