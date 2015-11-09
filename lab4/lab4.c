#include <minix/drivers.h>

#include "test4.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv) {
	sef_startup();
	sys_enable_iop(SELF);

	if (argc == 1) {
		print_usage(argv);
		return 0;
	} else {
		proc_args(argc, argv);
	}
	return 0;
}

static void print_usage(char *argv[]) { //  quando chamado para testar mostra as funcoes que podem ser testadas
	printf("Usage: one of the following:\n"
			"\t service run %s -args \"test_packet <cnt>\" \n"
			"\t service run %s -args \"test_async <idle_time>\" \n"
			"\t service run %s -args \"test_config\" \n"
			"\t service run %s -args \"test_gesture <length> <tolerance>\" \n",
			argv[0], argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) { //chama a funcao correspondente

	unsigned short cnt, idle_time, tolerance;
	short length;

	/* check the function to test: if the first characters match, accept it */
	if (strncmp(argv[1], "test_packet", strlen("test_packet")) == 0) {
		if (argc != 3) {
			printf("Mouse wrong no of arguments for test of test_packet()\n");
			return 1;
		}
		if ((cnt = parse_ulong(argv[2], 10)) == USHRT_MAX)
			return 1;
		printf("Mouse::test_packet(%u)\n", (unsigned) cnt);
		if (test_packet(cnt) == 0) {
			return 0;
		} else {
			return 1;
		}
	} else if (strncmp(argv[1], "test_async", strlen("test_async")) == 0) {
		if (argc != 3) {
			printf(
					"Mouse: wrong no of arguments for test of kbd_test_leds() \n");
			return 1;
		}
		if ((idle_time = parse_ulong(argv[2], 16)) == ULONG_MAX)
			return 1;
		printf("Mouse:: idle_time(%u)\n", (unsigned) idle_time);
		return test_async(idle_time);
	} else if (strncmp(argv[1], "test_config", strlen("test_config")) == 0) {
		if (argc != 2) {
			printf("Mouse: wrong no of arguments for test of vt_blank() \n");
			return 1;
		}
		printf("Mouse:: test_config\n");
		return test_config();
	}else if (strncmp(argv[1], "test_gesture", strlen("test_gesture")) == 0) {
		if( argc != 4 ) {
			printf("Mouse: wrong no of arguments for test of test_gesture() \n");
			return 1;
		}
		if((length = parse_ulong(argv[2], 16)) == ULONG_MAX )
		return 1;
		if((tolerance = parse_ulong(argv[3], 16)) == ULONG_MAX )
		return 1;
		printf("Mouse::test_gesture(%u,%u)\n",
				(unsigned)length,(unsigned)tolerance);
		return test_gesture(length,tolerance);
	} else {
		printf("Mouse: non valid function \"%s\" to test\n", argv[1]);
		return 1;
	}
}

static unsigned long parse_ulong(char *str, int base) {
	char *endptr;
	unsigned long val;

	val = strtoul(str, &endptr, base);

	if ((errno == ERANGE && val == ULONG_MAX) || (errno != 0 && val == 0)) {
		perror("strtol");
		return ULONG_MAX;
	}

	if (endptr == str) {
		printf("Mouse: parse_ulong: no digits were found in %s \n", str);
		return ULONG_MAX;
	}
	return val;
}
