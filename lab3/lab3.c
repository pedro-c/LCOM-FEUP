#include <minix/syslib.h>
#include <minix/drivers.h>

#include "test3.h"

static int proc_args(int argc, char *argv[]);
static unsigned long parse_ulong(char *str, int base);
static void print_usage(char *argv[]);

int main(int argc, char **argv)
{
		sef_startup();
		sys_enable_iop(SELF);

		if ( argc == 1 ) {
		      print_usage(argv);
		      return 0;
		  } else {
		      proc_args(argc, argv);
		  }
		  return 0;
}

static void print_usage(char *argv[]){ //  quando chamado para testar mostra as funcoes que podem ser testadas
  printf("Usage: one of the following:\n"
	 "\t service run %s -args \"kbd_test_scan <asm>\" \n"
	 "\t service run %s -args \"kbd_test_leds <n> <toggle>\" \n"
	 "\t service run %s -args \"kbd_test_timed_scan <n>\" \n",
	 argv[0], argv[0], argv[0]);
}

static int proc_args(int argc, char *argv[]) { //chama a funcao correspondente


  unsigned short ASM,n,leds,*toggle;

  /* check the function to test: if the first characters match, accept it */
  if (strncmp(argv[1], "kbd_test_scan", strlen("kbd_test_scan")) == 0)
  {
 		if( argc != 3 )
 		{
 			printf("Keyboard wrong no of arguments for test of timer_test_square()\n");
 			return 1;
 		}
 		if( (ASM = parse_ulong(argv[2], 10)) == USHRT_MAX)
 			return 1;
 		printf("KBD::kbd_test_scan(%u)\n",(unsigned)ASM);
 		if(kbd_test_scan(ASM) == 0)
 		{
 			return 0;
 		}
 		else
 		{
 			return 1;
 		}

  }
  else if (strncmp(argv[1], "kbd_test_leds", strlen("kbd_test_leds")) == 0) {
    	  if( argc != 4 ) {
    		  printf("KBD: wrong no of arguments for test of kbd_test_leds \n");
    		  return 1;
    	  }
    	  if( (leds = parse_ulong(argv[2], 10)) == ULONG_MAX)
    		  return 1;

    	  *toggle= malloc((argc-2)*sizeof(unsigned short));
    	  n=argc-2;

    	  if( (*toggle = parse_ulong(argv[3], 10)) == ULONG_MAX)
    		  return 1;

    	  unsigned int i=0;
    	  while(i<n)
    	  {
    		 if((toggle[i]=parse_ulong(argv[i+2],10))==ULONG_MAX || toggle[i]>2 || toggle[i]<0){
    			  printf("parameters must be between 0 and 2!");
    			  return 1;
    		  }

    		  i++;
    	  }
  	  return kbd_test_leds(n,toggle);
    }
  else if (strncmp(argv[1], "kbd_test_timed_scan", strlen("kbd_test_timed_scan")) == 0)
  {
	  if( argc != 3)
	  {
		  printf("KBD: wrong no of arguments for test of kbd_test_timed_scan() \n");
		  return 1;
	  }
	  if((n=parse_ulong(argv[2],10))==USHRT_MAX)
	  {
		  return 1;
	  }
	  printf("KBD::kbd_test_timed_scan(%u)\n",n);
	 if(kbd_test_timed_scan(n)==0)
	 {
		 return 0;
	 }
	 else
	 {
		 return 1;
	 }
  } else {
	  printf("KBD: non valid function \"%s\" to test\n", argv[1]);
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
	  printf("KBC: parse_ulong: no digits were found in %s \n", str);
	  return ULONG_MAX;
  }

  /* Successful conversion */
  return val;
}
