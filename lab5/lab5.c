#include <minix/drivers.h>

#include "video_gr.h"
#include "test5.h"
#include "pixmap.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {

	sef_startup();
	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}

static void print_usage(char *argv[]) {

	printf(
			"\t service run %s -args \"test_init <mode> <delay> \n"
					"\t service run %s -args \"test_square <x> <y> <size> <color> \n"
					"\t service run %s -args \"test_line <xi> <yi> <xf> <yf> <color> \n"
					"\t service run %s -args \"test_xpm <xi> <yi> <xpm[]> \n"
					"\t service run %s -args \"test_move <xi> <yi> <xpm[]> <hor> <delta> <time> \n"
					"\t service run %s -args \"test_controller \n", argv[0],
			argv[0], argv[0], argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {
	unsigned short mode, delay, x, y, size, xi, yi, xf,
			yf, hor, time;
	unsigned long color;
	short delta;
	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {
		if (argc != 4)
				{
					printf("wrong no of arguments for test_init()\n");
					return 1;
				}
				if ((mode = (unsigned short) parse_ulong(argv[2], 16)) == LONG_MAX)
					return 1;
				if ((delay = (unsigned short) parse_ulong(argv[3], 10)) == LONG_MAX)
					return 1;

				printf("test_init(0x%x, %d)\n", mode, delay);
				test_init(mode, delay);

				return 0;
	} else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {

		if (argc != 6) {
			printf("wrong no of arguments for test of test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) \n");
			return 1;
		}
		if ((x = (unsigned short) parse_ulong(argv[2], 10)) == LONG_MAX)
			return 1;
		if ((y = (unsigned short) parse_ulong(argv[3], 10)) == LONG_MAX)
			return 1;
		if ((size = (unsigned short) parse_ulong(argv[4], 10)) == LONG_MAX)
			return 1;
		if ((color = (unsigned long) parse_ulong(argv[5], 10)) == LONG_MAX)
			return 1;
		printf("test5::test_square(%d, %d, %d, %d)\n", x, y, size, color);
		return test_square(x, y, size, color);

	} else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {

		if (argc != 7) {

			printf("wrong no of arguments for test of test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) \n");
			return 1;
		}
		if ((xi = parse_ulong(argv[2], 10)) == LONG_MAX)
			return 1;
		if ((yi =parse_ulong(argv[3], 10)) == LONG_MAX)
			return 1;
		if ((xf = parse_ulong(argv[4], 10)) == LONG_MAX)
			return 1;
		if ((yf = parse_ulong(argv[5], 10)) == LONG_MAX)
			return 1;
		if ((color = parse_ulong(argv[6], 10)) == LONG_MAX)
			return 1;
		printf("test5::test_line(%d, %d, %d %d, %d)\n", xi, yi, xf, yf, color);
		return test_line(xi, yi, xf, yf, color);

	} else if (strncmp(argv[1], "test_xpm", strlen("test_xpm")) == 0) {
		if (argc != 5) {
			printf("wrong no of arguments for test of test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) \n");
			return 1;
		}
		if ((xi = parse_ulong(argv[2], 10)) == LONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == LONG_MAX)
			return 1;
		if(strncmp(argv[4], "pic1", strlen("pic1"))==0)
		{
			return test_xpm(xi, yi,pic1);
		}
	} else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {
		if (argc != 8) {
			printf(
					"wrong no of arguments for test of test_move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time) \n");
			return 1;
		}
		if ((xi = parse_ulong(argv[2], 10)) == LONG_MAX)
			return 1;
		if ((yi = parse_ulong(argv[3], 10)) == LONG_MAX)
			return 1;
		if ((hor = parse_ulong(argv[5], 10)) == LONG_MAX)
			return 1;
		if ((delta = parse_ulong(argv[6], 10)) == LONG_MAX)
			return 1;
		if ((time = parse_ulong(argv[7], 10)) == LONG_MAX)
			return 1;
		if(strncmp(argv[4], "pic1", strlen("pic1"))==0)
		{
			return test_move(xi,yi,pic1,hor,delta,time);
		}
	} else if (strncmp(argv[1], "test_controller", strlen("test_controller"))
			== 0) {
		if (argc != 2) {
			printf("wrong no of arguments for test of test_controller() \n");
			return 1;
		}
		printf("test_controller()\n");
		return test_controller();

	} else {
		printf("non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtol(str, &endptr, base);

	if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN))
			|| (errno != 0 && val == 0)) {
		perror("strtol");
		return LONG_MAX;
	}

	if (endptr == str) {
		printf("test5: parse_long: no digits were found in %s \n", str);
		return LONG_MAX;
	}

	/* Successful conversion */
	return val;
}

