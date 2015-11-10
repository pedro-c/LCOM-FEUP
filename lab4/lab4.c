#include <minix/drivers.h>

#include <minix/syslib.h>

#include "test4.h"

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

	printf("\t service run %s -args \"test_packet <packets_count>\" \n"

			"\t service run %s -args \"test_async <idle_time> \n"

			"\t service run %s -args \"test_config \n"

			"\t service run %s -args \"test_gesture <length> <tolerance> \n"

	, argv[0], argv[0], argv[0], argv[0]);

}

static int proc_args(int argc, char *argv[]) {

	unsigned short count, idle_time, tolerance;

	short length;

	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {

		if (argc != 3) {

			printf(
					"test4: wrong no of arguments for test of test_packet(unsigned short cnt)\n");

			return 1;

		}

		if ((count = parse_ushort(argv[2], 10)) == USHRT_MAX)

			return 1;

		printf("test4::test_packet(%u)\n", count);

		return test_packet(count);

		///

	} else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {

		if (argc != 3) {

			printf("mouse: wrong no of arguments for test of test_async(unsigned short idle_time) \n");
			return 1;
		}
		if ((idle_time = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		printf("test4::test_async(%u)\n", idle_time);
		return test_async(idle_time);
	} else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) {
		if (argc != 2) {
			printf("mouse: wrong no of arguments for test of test_config() \n");
			return 1;
		}
		printf("mouse::test_config()\n");
		return test_config();
	} else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {
		if (argc != 4) {
			printf("mouse: wrong no of arguments for test of test_gesture(short length, unsigned short tolerance) \n");
			return 1;
		}
		if ((length = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		if ((tolerance = parse_ushort(argv[2], 10)) == USHRT_MAX)
			return 1;
		printf("mouse::test_gesture(%d, %u)\n", length, tolerance);
		return test_gesture(length, tolerance);
	} else {
		printf("mouse: non valid function \"%s\" to test\n", argv[1]);
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
		printf("mouse: parse_ushort: no digits were found in %s \n", str);
		return USHRT_MAX;
	}
	return val;
}
