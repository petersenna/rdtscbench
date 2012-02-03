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

#include "rdtscbench.h"

/* General tool to measure run time of *function */
void inline rdtscb_measure (char msg[32], int repeats, int detailed, void (*function)( struct timespec *ts, struct timespec *te ) ){

	struct timespec begin, end, diff;
	long double acc = 1;
	int i = 0;

	printf ("%s: ", msg);

	for ( i = repeats; i > 0; i-- ){

		//getticks ( &begin );
		function( &begin, &end );
		//getticks ( &end );

		diff.tv_nsec = end.tv_nsec - begin.tv_nsec;

		acc *= (long)(double)diff.tv_nsec;
		if ( detailed == 1 )
			printf ( " %llu, ", diff.tv_nsec );
	}
	long double power = 1;
	power /= repeats;

	long double valuemean = powl ( acc, power );
	printf ("%25.6Lf clocks. Geom. mean of %4.0d values.\n", valuemean, repeats );
}

/* Assembly code to read the TSC */
__inline__ void rdtscb_getticks(struct timespec *ts) {
	unsigned int a, d;

	asm("cpuid"); /* disables out of order execution engine */

	asm volatile("rdtsc" : "=a" (a), "=d" (d));

	ts->tv_nsec = (((uint64_t)a) | (((uint64_t)d) << 32));
}

/* Accuracy detection
 * How many clock cycles in a second? It should be exactly the speed of your 
 *	CPU. Example: On a 2.7GHZ CPU it should count 2.700.000.000 clock 
 *	cycles. If it does, you got 100% accuracy.
 */
long double rdtscb_accuracy ( int repeats, int detailed ){
	struct timespec begin, end, diff;
	long double accuracy = 0, acc = 1;
	int i;

	printf ("\n----------------\n\naccuracy       :");

	for ( i = repeats; i > 0; i-- ){

		rdtscb_getticks ( &begin );
		sleep ( 1 );
		rdtscb_getticks ( &end );

		diff.tv_nsec = (long)(double) (end.tv_nsec - begin.tv_nsec);
		acc *= diff.tv_nsec;
		if ( detailed == 1 ){
			accuracy = diff.tv_nsec;
			accuracy *= 100;
			accuracy /= CPU_HZ;
			if ( accuracy > 100 ){
				accuracy -= 100;
				accuracy = 100 - accuracy;
			}
			printf ( "%2.6Lf, ", accuracy );
		}
	}
	long double power = 1;
	power /= repeats;

	accuracy = powl ( acc, power );

	accuracy *= 100;
	accuracy /= CPU_HZ;

	if ( accuracy > 100 ){
		accuracy -= 100;
		accuracy = 100 - accuracy;
	}

	printf ("  %25.6Lf%%       Geom. mean of %4.0d values.\n\n", accuracy, repeats );

	return accuracy;
}
