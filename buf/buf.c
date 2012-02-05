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

#define VIDEO_MAX_FRAME               32

enum vb2_buffer_state {
        VB2_BUF_STATE_DEQUEUED,
        VB2_BUF_STATE_QUEUED,
        VB2_BUF_STATE_ACTIVE,
        VB2_BUF_STATE_DONE,
        VB2_BUF_STATE_ERROR,
};

struct vb2_buffer {
	enum vb2_buffer_state	state;
};

struct vb2_queue {
	struct vb2_buffer		*bufs[VIDEO_MAX_FRAME];
	unsigned int			num_buffers;
};

void inline BUF1	( struct timespec *ts, struct timespec *te );
void inline BUF2	( struct timespec *ts, struct timespec *te );

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

	rdtscb_measure ( "buf1[]       ", repeats, detailed, &BUF1 );

	sleep(5); /* optional */

	rdtscb_measure ( "buf2[]       ", repeats, detailed, &BUF2 );

	sleep(5); /* optional */


	/* How to use accuracy() function */
	rdtscb_accuracy (accrepeats, detailed);
}

void inline BUF1 ( struct timespec *ts, struct timespec *te ){

	unsigned int i;

	struct vb2_queue *q;

	q = malloc(sizeof(struct vb2_queue));

	q->num_buffers = VIDEO_MAX_FRAME;

	for (i = 0; i < VIDEO_MAX_FRAME; ++i)
		q->bufs[i] = (struct vb2_buffer *) malloc(sizeof(struct vb2_buffer));

	rdtscb_getticks ( ts ); /* start measurement */

	for (i = 0; i < q->num_buffers; ++i)
		q->bufs[i]->state = VB2_BUF_STATE_DEQUEUED;

	rdtscb_getticks ( te ); /* end measurement */
}

void inline BUF2 ( struct timespec *ts, struct timespec *te ){

	unsigned int i;

	struct vb2_queue *q;

	struct vb2_buffer *buf_ptr, *buf_ptr_end;

	q = malloc(sizeof(struct vb2_queue));

	q->num_buffers = VIDEO_MAX_FRAME;

	for (i = 0; i < VIDEO_MAX_FRAME; ++i)
		q->bufs[i] = (struct vb2_buffer *) malloc(sizeof(struct vb2_buffer));

	rdtscb_getticks ( ts ); /* start measurement */

	buf_ptr_end = q->bufs[q->num_buffers];

	for (buf_ptr = q->bufs[0]; buf_ptr < buf_ptr_end; ++buf_ptr){
		buf_ptr->state = VB2_BUF_STATE_DEQUEUED;
	}

	rdtscb_getticks ( te ); /* end measurement */
}

