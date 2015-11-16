#include <minix/drivers.h>
#include <minix/syslib.h>
#include "test5.h"

static int proc_args(int argc, char *argv[]);
static unsigned short parse_ushort(char *str, int base);
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

	printf("\t service run %s -args \"test_init <mode> <delay> \n"

			"\t service run %s -args \"test_square <x> <y> <size> <color> \n"

			"\t service run %s -args \"test_line <xi> <yi> <xf> <yf> <color> \n"

			"\t service run %s -args \"test_xpm <xi> <yi> <xpm[]> \n"

			"\t service run %s -args \"test_move <xi> <yi> <xpm[]> <hor> <delta> <time> \n"

			"\t service run %s -args \"test_controller \n"

	, argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {

	unsigned short count, idle_time, tolerance;

	short length;

	if (strncmp(argv[1], "test_init", strlen("test_init")) == 0) {

		if (argc != 3) {

			printf(
					"test5: wrong no of arguments for test of test_init(unsigned short mode, unsigned short delay) \n");

			return 1;

		}

		if ((count = parse_ushort(argv[2], 10)) == USHRT_MAX)

			return 1;

		if ((count = parse_ushort(argv[2], 10)) == USHRT_MAX)

			return 1;

		printf("test5::test_init(%u, %d)\n", mode, delay);

		return test_init(mode, delay);

		///

	} else if (strncmp(argv[1], "test_square", strlen("test_square")) == 0) {

		if (argc != 3) {

			printf("wrong no of arguments for test of test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) \n");
			return 1;
		}
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		printf("test5::test_square(%u, %d, %x %y)\n", x, y, size, color);
		return test_square(x,y,size, color);

	} else if (strncmp(argv[1], "test_line", strlen("test_line")) == 0) {

		if (argc != 3) {

			printf("wrong no of arguments for test of test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) \n");
			return 1;
		}
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		printf("test5::test_line(%u, %d, %x %y, %z)\n", xi, yi, xf, yf, color);
		return test_line(xi, yi, xf, yf, color);

	} else if (strncmp(argv[1], "test_xpm", strlen("xpm")) == 0) {
		if (argc != 4) {
			printf("wrong no of arguments for test of test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) \n");
			return 1;
		}
		if ((length = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((tolerance = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((tolerance = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		printf("test_xpm(%d, %u, %z)\n", xi, yi, xmp);
		return test_xpm( xi, yi, xmp);

	} else if (strncmp(argv[1], "test_move", strlen("test_move")) == 0) {

			if (argc != 3) {

				printf("wrong no of arguments for test of test_move(unsigned short xi, unsigned short yi, char *xpm[], unsigned short hor, short delta, unsigned short time) \n");
				return 1;
			}
			if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
				return 1;
			if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
				return 1;
			if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
				return 1;
			if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
				return 1;
			if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
				return 1;
			if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
				return 1;
			printf("test5::test_line(%u, %d, %x %y, %z, %w)\n", xi, yi, xpm, hor, delta, time);
			return test_move(xi, yi, xpm, hor, delta, time);

	} else if (strncmp(argv[1], "test_controler", strlen("test_controler")) == 0) {
			if (argc != 2) {
				printf("wrong no of arguments for test of test_controller() \n");
				return 1;
			}
			printf("test_controller()\n");
			return test_controler();

	} else {
		printf("non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

unsigned short parse_ushort(char *str, int base) {

	char *endptr;
	unsigned short val;
	val = strtoul(str, &endptr, base);
	if ((errno == ERANGE && val == USHRT_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return USHRT_MAX;
	}
	if (endptr == str) {
		printf("parse_ushort: no digits were found in %s \n", str);
		return USHRT_MAX;
	}
	return val;
}
