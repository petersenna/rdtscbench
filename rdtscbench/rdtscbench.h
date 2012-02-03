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

#include "hzdef.h" /* for CPU_HZ */

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * rdtscb_measure (char msg[32], int repeats, int detailed, \
 *	void (*function)( struct timespec *ts, struct timespec *te ) );
 *
 * Call the function to be measured, calculate the average mean and print
 *	results on screen.
 *
 * char msg[32]	- Message to print on screen related to test
 *
 * int repeats	- How many times to repeat the test? Ex.: 256
 *
 * int detailed	- Print detailed information of all tests? Set to > 0 to yes.
 *
 * void (*function)( struct timespec *ts, struct timespec *te )	- Function 
 *	to be measured. See GETPID() example.
 *
 */
void inline rdtscb_measure (char msg[32], int repeats, int detailed, \
	void (*function)( struct timespec *ts, struct timespec *te ) );

/*
 * rdtscb_getticks(struct timespec *ts)
 * saves the result of RDTSC instruction at *ts
 */
__inline__ void rdtscb_getticks(struct timespec *ts);

/*
 * accuracy(int repeats, int detailed);
 *
 * Calculates the accuracy of the measurement and prints it on screen.
 *
 * int repeats	- How many times to repeat the test? Ex.: 8
 *
 * int detailed	- Print detailed information of all tests? Set to > 0 to yes.
 *
 * RETURNS: The geometric mean of the accuracy
 */
long double rdtscb_accuracy(int repeats, int detailed);
