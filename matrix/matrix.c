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

#include "matrix.h"

#define X_SIZE	60
#define Y_SIZE	30


int matrix[X_SIZE][Y_SIZE];

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

	rdtscb_measure ( "matrix1()       ", repeats, detailed, &MATRIX1 );
	sleep(5);

	rdtscb_measure ( "matrix2()       ", repeats, detailed, &MATRIX2 );
	sleep(5);

	rdtscb_measure ( "matrix3()       ", repeats, detailed, &MATRIX3 );
	sleep(5);

	rdtscb_measure ( "matrix4()       ", repeats, detailed, &MATRIX4 );
	sleep(5);

	rdtscb_measure ( "matrix5()       ", repeats, detailed, &MATRIX5 );
	sleep(5);

	rdtscb_measure ( "matrix6()       ", repeats, detailed, &MATRIX6 );
	sleep(5);

	rdtscb_measure ( "matrix7()       ", repeats, detailed, &MATRIX7 );
	sleep(5);

	rdtscb_measure ( "matrix8()       ", repeats, detailed, &MATRIX8 );
	sleep(5);

	rdtscb_accuracy (accrepeats, detailed );

}

/*
 * MATRIX 1
 */

void inline MATRIX1 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	int x, y;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	for (x = 0; x < X_SIZE; ++x){
		for (y = 0; y < Y_SIZE; ++y){
			matrix[x][y] = -1;
		}
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 2
 */
void inline MATRIX2 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	register int x,y;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	for (x = 0; x < X_SIZE; ++x){
		for (y = 0; y < Y_SIZE; ++y){
			matrix[x][y] = -1;
		}
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 3
 */
void inline MATRIX3 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	register int x,y;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	for (y = 0; y < Y_SIZE; ++y){
		for (x = 0; x < X_SIZE; ++x){
			matrix[x][y] = -1;
		}
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 4
 */
void inline MATRIX4 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	32

	register int x,y;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	for (y = 0; y < Y_SIZE; ++y){
		for (x = 0; x < X_SIZE; ++x){
			matrix[x][y] = -1;
		}
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 5
 */
void inline MATRIX5 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	register int index;
	register int *matrix_ptr;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	matrix_ptr = &matrix[0][0];

	for (index = 0; index < X_SIZE * Y_SIZE; ++index){
		*matrix_ptr = -1;
		matrix_ptr++;
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 6
 */
void inline MATRIX6 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	register int *matrix_ptr;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	for (matrix_ptr = &matrix[0][0];
		matrix_ptr <= &matrix[X_SIZE - 1][Y_SIZE - 1];
		++matrix_ptr){
		*matrix_ptr = -1;
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 7 
 */
void initmatrix(){
	memset (matrix, -1, sizeof(matrix));
}

void inline MATRIX7 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	register int *matrix_ptr;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	initmatrix();

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

/*
 * MATRIX 8
 */
#define init_matrix() \
	memset (matrix, -1, sizeof(matrix));

void inline MATRIX8 (struct timespec *ts, struct timespec *te){
	#define X_SIZE	60
	#define Y_SIZE	30

	register int *matrix_ptr;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	init_matrix();

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}
