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
#include <sys/time.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>


/* Functions used to benchmark some code */
void inline GETTIMEOFDAY( struct timespec *ts, struct timespec *te );
void inline SYSCONF	( struct timespec *ts, struct timespec *te );
void inline SIMPLEMATH	( struct timespec *ts, struct timespec *te );
void inline MMEM	( struct timespec *ts, struct timespec *te );
void inline GMEM	( struct timespec *ts, struct timespec *te );
void inline PIPES	( struct timespec *ts, struct timespec *te );

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

	rdtscb_measure ( "sysconf()      ", repeats, detailed, &SYSCONF );
	sleep(5);

	rdtscb_measure ( "gettimeofday() ", repeats, detailed, &GETTIMEOFDAY );
	sleep(5);

	rdtscb_measure ( "malloc[1M]     ", repeats, detailed, &MMEM );
	sleep(5);

	rdtscb_measure ( "malloc[1G]     ", repeats, detailed, &GMEM );
	sleep(5);

	rdtscb_measure ( "2pipes[]       ", repeats, detailed, &PIPES );
	sleep(5);

	rdtscb_measure ( "simplemath[]   ", repeats, detailed, &SIMPLEMATH );
	sleep(5);

	rdtscb_accuracy (accrepeats, detailed );

}

void inline GETTIMEOFDAY ( struct timespec *ts, struct timespec *te ){
	struct timeval tv;
	struct timezone tz;
	int time;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	time = gettimeofday ( &tv, &tz );	

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}


void inline SIMPLEMATH ( struct timespec *ts, struct timespec *te ){
	uint64_t i,a,b,c,d;
	a = 1, b = 2, c = 4, d = 8;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	for (i = 0; i < 100000LL; i++){
		a = ( ( d * c ) / b);
		a--;b++;c++;d++;
	}

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

void inline SYSCONF ( struct timespec *ts, struct timespec *te ){
	long conf;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	conf = sysconf(_SC_ARG_MAX);

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

}

void inline MMEM ( struct timespec *ts, struct timespec *te ){
	unsigned int size = 1000000;
	char *mem;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	mem = malloc ( size * sizeof ( char ) );

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

	free ( mem );
}

void inline GMEM ( struct timespec *ts, struct timespec *te ){
	unsigned int size = 1000000000;
	char *mem;

	/* ### starts measuring ### */
	rdtscb_getticks ( ts );

	mem = malloc ( size * sizeof ( char ) );

	/* ### ends measuring ### */
	rdtscb_getticks ( te );

	free ( mem );
}

/* Parent writes on pipePtoC. Child reads it and writes back on pipeCtoP */
void inline PIPES ( struct timespec *ts, struct timespec *te ){
	char send[16]="message", recive[16];
	int pipePtoC[2], pipeCtoP[2], i;
	pid_t cpid;
	char buf;

	if ((pipe(pipePtoC) == -1) || (pipe(pipeCtoP) == -1)) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	for ( i=0; i < sizeof(recive); i++ )
		recive[i] = '\0';

	cpid = fork();
	if (cpid == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	/* Child */
	if (cpid == 0) {
		close(pipePtoC[1]);
		close(pipeCtoP[0]);

		/* recive message from parent and send it back */
		while (read(pipePtoC[0], &buf, 1) > 0)
			write(pipeCtoP[1], &buf, 1);

		write(pipeCtoP[1], "\n", 1);

		close(pipePtoC[0]);
		close(pipeCtoP[1]);

		_exit(EXIT_SUCCESS);

	/* Parent */
	} else {
		close(pipePtoC[0]);
		close(pipeCtoP[1]);

		/* ### starts measuring ### */
		rdtscb_getticks ( ts );

		/* Sendig send[16] */
		write(pipePtoC[1], send, strlen(send));
		close(pipePtoC[1]);

		/* Receiving it back and saving on recive[16]*/
		i=0;
		while (read(pipeCtoP[0], &buf, 1) > 0){
			strncat(recive, &buf, 1);
			i++;
		}
		recive[(i-1)]='\0';

		if ( strcmp ( send, recive ) != 0 )
			exit (1);

		/* ### ends measuring ### */
		rdtscb_getticks ( te );

		close(pipePtoC[0]);
		close(pipeCtoP[0]);

		wait(NULL);
	}
}
