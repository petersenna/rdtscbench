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
#include <memory.h> /* MATRIX7 and MATRIX8 memset() */

/* Functions used to benchmark some code */
void MATRIX1	( struct timespec *ts, struct timespec *te );
void MATRIX2	( struct timespec *ts, struct timespec *te );
void MATRIX3	( struct timespec *ts, struct timespec *te );
void MATRIX4	( struct timespec *ts, struct timespec *te );
void MATRIX5	( struct timespec *ts, struct timespec *te );
void MATRIX6	( struct timespec *ts, struct timespec *te );

void initmatrix();	/* used by MATRIX7() */
void MATRIX7	( struct timespec *ts, struct timespec *te );
void MATRIX8	( struct timespec *ts, struct timespec *te );
