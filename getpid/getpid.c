/* rdtscbench: measures operations in clock cycles using RDTSC x86 instruction
 *
 * Peter Senna Tschudin - peter.senna@gmail.com
 *
 * IMPORTANT: Before running make and rdtscbench disable CPU dynamic clock.
 *	 On Linux: # service cpuspeed stop
 *
 * IMPORTANT 2: If the computer hybernates during the measurement, it may 
 *	break the measurement.
 *
 * The Makefile runs gethz.sh to cat /proc/cpuinfo looking for the CPU speed.
 *	The result is saved in hzdef.h. See example:
 *	$ cat /proc/cpuinfo |grep MHz
 *	cpu MHz		: 2701.000
 *	gethz.sh converts 2701.000 to 2701000000LL and saves it at hzdef.h
 *
 */

#include "../rdtscbench/rdtscbench.h"

/* 
 * Sample function that shows how to use rdtscbench to measure operations
 */
void GETPID	( struct timespec *ts, struct timespec *te );

int main (int argc, char *argv[]){
	struct timespec begin, end;
	int accrepeats, repeats, detailed = 0;

	if ( argc < 3 ){
		printf ("Error. You need to specify two numbers as arguments.\n\n %s <repeats> <acc repeats> <optional detailed run parameter. Try 1>\n\n" , argv[0]);
		exit (1);
	}
	repeats = atoi(argv[1]);
	accrepeats = atoi(argv[2]);

	if ( argc > 3 )
		detailed = 1;

	/* How to use measure() function */
	rdtscb_measure ( "getpid()       ", repeats, detailed, &GETPID );

	sleep(5); /* optional */

	/* How to use accuracy() function */
	rdtscb_accuracy (accrepeats, detailed);
}

/*
 * Sample function
 *
 * add "getticks ( ts );" before what you want to measure. "s" from ts is start
 *
 * add "getticks ( te );" after what you want to measure. "e" from te is end
 */
void inline GETPID ( struct timespec *ts, struct timespec *te ){

	pid_t pid; /* time to do this is intentionally not measured */

	rdtscb_getticks ( ts ); /* start measurement */

	pid = getpid(); /* action to be measured */

	rdtscb_getticks ( te ); /* end measurement */

}
