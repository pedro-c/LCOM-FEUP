#include <minix/syslib.h>
#include <minix/drivers.h>

#include "timer.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv)
{
		sef_startup();

		if ( argc == 1 ) {
		      print_usage(argv);
		      return 0;
		  } else {
		      proc_args(argc, argv);
		  }
		  return 0;
	}

static void print_usage(char *argv[]) {
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"timer_test_square <freq>\" \n"
	 "\t service run %s -args \"timer_test_config <timer>\" \n"
	 "\t service run %s -args \"timer_test_int <time>\" \n",
	 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) {


  unsigned long timer,freq,time;
  char *str;
  long num;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "timer_test_square", strlen("timer_test_square")) == 0)
  {
 		if( argc != 3 )
 		{
 			printf("timer wrong no of arguments for test of timer_test_square()\n");
 			return 1;
 		}
 		if( (freq = parse_ulong(argv[2], 10)) == ULONG_MAX )
 			return 1;
 		printf("timer::timer_test_square(%u)\n",(unsigned)freq);
 		if(timer_test_square(freq) == 0)
 		{
 			printf("Done!\n");
 			return 1;
 		}
 		else
 		{
 			printf("Error!\n");
 		}

  }
  else if (strncmp(argv[1], "timer_test_config", strlen("timer_test_config")) == 0)
  {
	  if( argc != 3 )
	  {
		  printf("timer: wrong no of arguments for test of timer_tes_config() \n");
		  return 1;
	  }
	  if((timer=parse_ulong(argv[2],10)) == ULONG_MAX)
		  return 1;
	  printf("timer:: timer_test_config(%u)\n",(unsigned) timer);
	  if(timer_test_config(timer)==0)
	  {
		  printf("Done !\n");
	  }
	  else
	  {
		  printf("Error!\n");
	  }
  }
  else if (strncmp(argv[1], "timer_test_int", strlen("timer_test_int")) == 0)
  {
	  if( argc != 3)
	  {
		  printf("timer: wrong no of arguments for test of timer_test_int() \n");
		  return 1;
	  }
	  if((time=parse_ulong(argv[2],10))==ULONG_MAX)
	  {
		  return 1;
	  }
	  printf("timer::timer_test_int(%u)\n",time);
	 if(timer_test_int(time)==0)
	 {
		 printf("Done !\n");
	 }
	 else
	 {
		 printf("Error !\n");
	 }
  } else {
	  printf("timer: non valid function \"%s\" to test\n", argv[1]);
	  return 1;
  }
}

static unsigned long parse_ulong(char *str, int base) {
  char *endptr;
  unsigned long val;

  val = strtoul(str, &endptr, base);

  if ((errno == ERANGE && val == ULONG_MAX )
	  || (errno != 0 && val == 0)) {
	  perror("strtol");
	  return ULONG_MAX;
  }

  if (endptr == str) {
	  printf("video_txt: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}
