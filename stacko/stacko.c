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
#include <stdlib.h>
#include <string.h>

void inline STACKO		( struct timespec *ts, struct timespec *te );
void inline STACKO1		( struct timespec *ts, struct timespec *te );
void inline STACKO2		( struct timespec *ts, struct timespec *te );
void inline STACKOmoonshadow	( struct timespec *ts, struct timespec *te );
void inline STACKOmoonshadow2	( struct timespec *ts, struct timespec *te );
/*void inline STACKOonemasse	( struct timespec *ts, struct timespec *te );*/
void inline STACKOChristoffer	( struct timespec *ts, struct timespec *te );

#define MAX 100

int main (int argc, char *argv[]){
	struct timespec begin, end;
	int i, accrepeats, repeats, detailed = 0;

	if ( argc < 3 ){
		printf ("Error. You need to specify two numbers as arguments.\n\n %s <repeats> <acc repeats> <optional detailed run parameter. Try 1>\n\n" , argv[0]);
		exit (1);
	}
	repeats = atoi(argv[1]);
	accrepeats = atoi(argv[2]);

	if ( argc > 3 )
		detailed = 1;

	rdtscb_measure ( "stacko[]           ", repeats, detailed, &STACKO );
	sleep(3); /* optional */
	rdtscb_measure ( "stacko1[]          ", repeats, detailed, &STACKO1 );
	sleep(3); /* optional */
	rdtscb_measure ( "stacko2[]          ", repeats, detailed, &STACKO2 );
	sleep(3); /* optional */
	rdtscb_measure ( "stackomoonshadow[] ", repeats, detailed, &STACKOmoonshadow );
	sleep(3); /* optional */
	rdtscb_measure ( "stackomoonshadow2[]", repeats, detailed, &STACKOmoonshadow2 );
	sleep(3); /* optional */
	rdtscb_measure ( "stackoChristoffer[]", repeats, detailed, &STACKOChristoffer );
	sleep(3); /* optional */

/*	rdtscb_measure ( "stackoonemasse[]", repeats, detailed, &STACKOonemasse );*/



	/* How to use accuracy() function */
	rdtscb_accuracy (accrepeats, detailed);
}

void inline STACKO ( struct timespec *ts, struct timespec *te ){

	int i, *s, *a, *b;

	for (i = 0; i < MAX; ++i){
		s = (int *) malloc (sizeof (int)); ++s;
		a = (int *) malloc (sizeof (int)); ++a;
		b = (int *) malloc (sizeof (int)); ++b;
	}

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

	rdtscb_getticks ( ts ); /* start measurement */

	for (i = 0; i < MAX; i++)
		s[i] = a[i] ^ b[i];

	rdtscb_getticks ( te ); /* end measurement */


	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");
}

void inline STACKO1 ( struct timespec *ts, struct timespec *te ){

	register int i, *s, *a, *b;

	srand ( 1024 );

	for (i = 0; i < MAX; ++i){
		s = (int *) malloc (sizeof (int)); ++s;
		a = (int *) malloc (sizeof (int)); ++a;
		b = (int *) malloc (sizeof (int)); ++b;
	}

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

	rdtscb_getticks ( ts ); /* start measurement */

	for (i = 0; i < MAX; i++)
		s[i] = a[i] ^ b[i];

	rdtscb_getticks ( te ); /* end measurement */

/*
	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");
*/
}

void inline STACKO2 ( struct timespec *ts, struct timespec *te ){

	register int i, *s, *s_ptr, *s_end, *a, *a_ptr, *b, *b_ptr;

	for (i = 0; i < MAX; ++i){
		s = (int *) malloc (sizeof (int)); ++s;
		a = (int *) malloc (sizeof (int)); ++a;
		b = (int *) malloc (sizeof (int)); ++b;
	}

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

	rdtscb_getticks ( ts ); /* start measurement */

	s_end = &s[MAX];
	for (s_ptr = &s[0], a_ptr = &a[0], b_ptr = &b[0]; s_ptr < s_end; ++s_ptr, ++a_ptr, ++b_ptr)
		*s_ptr = *a_ptr ^ *b_ptr;

	rdtscb_getticks ( te ); /* end measurement */

/*
	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");
*/
}

void inline STACKOmoonshadow ( struct timespec *ts, struct timespec *te ){

        int i, *s, *s_end, *a, *b, *s_ptr, *a_ptr, *b_ptr;

        for (i = 0; i < MAX; ++i){
                s = (int *) malloc (sizeof (int)); ++s;
                a = (int *) malloc (sizeof (int)); ++a;
                b = (int *) malloc (sizeof (int)); ++b;
        }

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

        rdtscb_getticks ( ts ); /* start measurement */
	
	s_ptr = &s[0];
	a_ptr = &a[0];
	b_ptr = &b[0];

	for (i = 0; i < (MAX/4); i++){

		s_ptr[0] = a_ptr[0] ^ b_ptr[0];
		s_ptr[1] = a_ptr[1] ^ b_ptr[1];
		s_ptr[2] = a_ptr[2] ^ b_ptr[2];
		s_ptr[3] = a_ptr[3] ^ b_ptr[3];
		s_ptr+=4; a_ptr+=4; b_ptr+=4;
	}

        rdtscb_getticks ( te ); /* end measurement */


	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");

}

void inline STACKOmoonshadow2 ( struct timespec *ts, struct timespec *te ){

        register int i, *s, *s_end, *a, *b, *s_ptr, *a_ptr, *b_ptr;

        for (i = 0; i < MAX; ++i){
                s = (int *) malloc (sizeof (int)); ++s;
                a = (int *) malloc (sizeof (int)); ++a;
                b = (int *) malloc (sizeof (int)); ++b;
        }

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

        rdtscb_getticks ( ts ); /* start measurement */
	
	s_ptr = &s[0];
	a_ptr = &a[0];
	b_ptr = &b[0];

	for (i = 0; i < (MAX/4); i++){

		s_ptr[0] = a_ptr[0] ^ b_ptr[0];
		s_ptr[1] = a_ptr[1] ^ b_ptr[1];
		s_ptr[2] = a_ptr[2] ^ b_ptr[2];
		s_ptr[3] = a_ptr[3] ^ b_ptr[3];
		s_ptr+=4; a_ptr+=4; b_ptr+=4;
	}

        rdtscb_getticks ( te ); /* end measurement */


	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");

}

void inline STACKOChristoffer ( struct timespec *ts, struct timespec *te ){

        int i, *s, *s_end, *a, *b;

        for (i = 0; i < MAX; ++i){
                s = (int *) malloc (sizeof (int)); ++s;
                a = (int *) malloc (sizeof (int)); ++a;
                b = (int *) malloc (sizeof (int)); ++b;
        }

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

        rdtscb_getticks ( ts ); /* start measurement */

	#pragma omp for
	for (i = 0; i < MAX; i++)
	{
		s[i] = a[i] ^ b[i];
	}

        rdtscb_getticks ( te ); /* end measurement */
/*
	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");
*/
}

/*
void inline STACKOonemasse ( struct timespec *ts, struct timespec *te ){

        register int i, *s, *a, *b;

        for (i = 0; i < MAX; ++i){
                s = (int *) malloc (sizeof (int)); ++s;
                a = (int *) malloc (sizeof (int)); ++a;
                b = (int *) malloc (sizeof (int)); ++b;
        }

	srand ( 1024 );
	for (i = 0; i < MAX; ++i){
		s[i] = a[i] = ( rand() % 2 );
		b[i] = ( rand() % 2 );
	}

        rdtscb_getticks ( ts ); 

	memcpy( s, a, MAX );

	for (i = 0; i < MAX; i++)
	{
		s[i] = s[i] ^ b[i];
	}

        rdtscb_getticks ( te );

	printf("\n");
	for (i = 0; i < MAX; ++i)
		printf("%d", s[i]);
	printf("\n");
}
*/
