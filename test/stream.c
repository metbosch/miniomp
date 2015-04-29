# include <stdio.h>
# include <math.h>
# include <float.h>
# include <stdlib.h>
//# include "limits.h"
# include <sys/time.h>
# include <omp.h>

/* INSTRUCTIONS:
 *
 *	1) Stream requires a good bit of memory to run.  Adjust the
 *          value of 'N' (below) to give a 'timing calibration' of 
 *          at least 20 clock-ticks.  This will provide rate estimates
 *          that should be good to about 5% precision.
 */

# define N	128*1024*512 
# define NTIMES	10
# define OFFSET	0

/*
 *	3) Compile the code with full optimization.  Many compilers
 *	   generate unreasonably bad code before the optimizer tightens
 *	   things up.  If the results are unreasonably good, on the
 *	   other hand, the optimizer might be too smart for me!
 *
 *         Try compiling with:
 *               cc -O stream_omp.c -o stream_omp
 *
 *         This is known to work on Cray, SGI, IBM, and Sun machines.
 *
 *
 *	4) Mail the results to mccalpin@cs.virginia.edu
 *	   Be sure to include:
 *		a) computer hardware model number and software revision
 *		b) the compiler flags
 *		c) all of the output from the test case.
 * Thanks!
 *
 */

# define HLINE "-------------------------------------------------------------\n"

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif
# ifndef MAX
# define MAX(x,y) ((x)>(y)?(x):(y))
# endif

#define OmpSs
//#define SMPSs
//#define CellSs
//#define CellSs_tracing
#define TUNED
// Definition of BSIZE for CellSs 
#ifdef CellSs
#define BSIZE 4000
#endif
// Definition of BSIZE for CellSs when using tracing
#ifdef CellSs_tracing
#define BSIZE 3200 
#endif
#ifdef SMPSs
// Definition of BSIZE for SMPSs
#define BSIZE N/64
#endif
#ifdef OmpSs
// Definitions of BSIZE for OmpSs
#define BSIZE N/64
#endif


static double	__attribute__((aligned(4096)))a[N+OFFSET],
		b[N+OFFSET],
		c[N+OFFSET];

static double	avgtime[4] = {0}, maxtime[4] = {0},
		mintime[4] = {FLT_MAX,FLT_MAX,FLT_MAX,FLT_MAX};

static char	*label[4] = {"Copy:      ", "Scale:     ",
    "Add:       ", "Triad:     "};

static double	bytes[4] = {
    2 * sizeof(double) * N,
    2 * sizeof(double) * N,
    3 * sizeof(double) * N,
    3 * sizeof(double) * N
    };

extern double mysecond();
extern void checkSTREAMresults();
#ifdef TUNED
extern void tuned_STREAM_Copy();
extern void tuned_STREAM_Scale(double scalar);
extern void tuned_STREAM_Add();
extern void tuned_STREAM_Triad(double scalar);
#endif

void init_task(double *a, double *b, double *c, int bs)
{
	int i, j;
        for (i=0; i < BSIZE; i += 512) {
        #pragma omp task private(j) firstprivate(i)	
	for (j=i; j < i + 512; j++){
	        a[j] = 1.0;
	        b[j] = 2.0;
	        c[j] = 0.0;
		a[j] = 2.0E0 * a[j];
  	}
        }
}

void tuned_initialization()
{
	int j;
        for (j=0; j<N; j+=BSIZE)
//Assumes N is multiple of BSIZE 
            init_task (&a[j], &b[j], &c[j], BSIZE); 
}

int
main(int argc, char *argv[])
    {
    int			quantum, checktick();
    int			BytesPerWord;
    register int	j, k;
    double		scalar, t, times[4][NTIMES], total_time;



    /* --- SETUP --- determine precision and check timing --- */

    printf(HLINE);
    printf("STREAM version $Revision: 5.8 $\n");
    printf(HLINE);
    BytesPerWord = sizeof(double);
    printf("This system uses %d bytes per DOUBLE PRECISION word.\n",
	BytesPerWord);

    printf(HLINE);
    printf("Array size = %d, Offset = %d\n" , N, OFFSET);
    printf("Total memory required = %.1f MB.\n",
	(3.0 * BytesPerWord) * ( (double) N / 1048576.0));
    printf("Each test is run %d times, but only\n", NTIMES);
    printf("the *best* time for each is used.\n");

#pragma omp parallel
{
#pragma omp single
{
#ifdef OmpSs
    printf(HLINE);
    k = omp_get_num_threads();
    printf ("Number of Threads = %i\n",k);
#endif
#ifdef SMPSs
    printf(HLINE);
    printf("CSS_NUM_CPUS %s \n", getenv("CSS_NUM_CPUS"));
    k = atoi (getenv ("CSS_NUM_CPUS"));
    printf ("Number of CSS Threads = %i\n",k);
#endif
#ifdef CellSs
    printf(HLINE);
    k = atoi (getenv ("CSS_NUM_SPUS"));
    printf ("Number of CSS Threads = %i\n",k);
#endif
#ifdef CellSs_tracing
    printf(HLINE);
    k = atoi (getenv ("CSS_NUM_SPUS"));
    printf ("Number of CSS Threads = %i\n",k);
#endif

    printf(HLINE);
    printf ("Printing one line per active thread....\n");
    /* Get initial value for system clock. */

/*
    for (j=0; j<N; j++) {
	a[j] = 1.0;
	b[j] = 2.0;
	c[j] = 0.0;
	}

*/
total_time = mysecond();
#pragma omp task
    tuned_initialization();
#pragma omp taskwait

    /*	--- MAIN LOOP --- repeat test cases NTIMES times --- */

    scalar = 3.0;
    for (k=0; k<NTIMES; k++)
	{
	times[0][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Copy();
#else
printf("WARNING: This version is a port to StarSs that only works for TUNED option \n");
	for (j=0; j<N; j++)
	    c[j] = a[j];
#endif
#pragma omp taskwait  
	times[0][k] = mysecond() - times[0][k];
	
	times[1][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Scale(scalar);
#else
	for (j=0; j<N; j++)
	    b[j] = scalar*c[j];
#endif
#pragma omp taskwait
	times[1][k] = mysecond() - times[1][k];
	
	times[2][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Add();
#else
	for (j=0; j<N; j++)
	    c[j] = a[j]+b[j];
#endif
#pragma omp taskwait 
	times[2][k] = mysecond() - times[2][k];
	
	times[3][k] = mysecond();
#ifdef TUNED
        tuned_STREAM_Triad(scalar);
#else
	for (j=0; j<N; j++)
	    a[j] = b[j]+scalar*c[j];
#endif
#pragma omp  taskwait
	times[3][k] = mysecond() - times[3][k];
	}
total_time = mysecond() - total_time;
#pragma omp taskwait
    /*	--- SUMMARY --- */

    for (k=1; k<NTIMES; k++) /* note -- skip first iteration */
	{
	for (j=0; j<4; j++)
	    {
	    avgtime[j] = avgtime[j] + times[j][k];
	    mintime[j] = MIN(mintime[j], times[j][k]);
	    maxtime[j] = MAX(maxtime[j], times[j][k]);
	    }
	}
    

    printf("Function      Rate (MB/s)   Avg time     Min time     Max time\n");
    for (j=0; j<4; j++) {
	avgtime[j] = avgtime[j]/(double)(NTIMES-1);

	printf("%s%11.4f  %11.4f  %11.4f  %11.4f\n", label[j],
	       1.0E-06 * bytes[j]/mintime[j],
	       avgtime[j],
	       mintime[j],
	       maxtime[j]);
    }

    printf(HLINE);

    printf("TOTAL time (including initialization) =  %11.4f seconds\n", total_time);
    /* --- Check Results --- */
    checkSTREAMresults();
    printf(HLINE);
} }
    return 0;
}

# define	M	20

int
checktick()
    {
    int		i, minDelta, Delta;
    double	t1, t2, timesfound[M];

/*  Collect a sequence of M unique time values from the system. */

    for (i = 0; i < M; i++) {
	t1 = mysecond();
	while( ((t2=mysecond()) - t1) < 1.0E-6 )
	    ;
	timesfound[i] = t1 = t2;
	}

/*
 * Determine the minimum difference between these M values.
 * This result will be our estimate (in microseconds) for the
 * clock granularity.
 */

    minDelta = 1000000;
    for (i = 1; i < M; i++) {
	Delta = (int)( 1.0E6 * (timesfound[i]-timesfound[i-1]));
	minDelta = MIN(minDelta, MAX(Delta,0));
	}

   return(minDelta);
    }



/* A gettimeofday routine to give access to the wall
   clock timer on most UNIX-like systems.  */

#include <sys/time.h>

double mysecond()
{
        struct timeval tp;
        int i;

        i = gettimeofday(&tp,NULL);
        return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}

void checkSTREAMresults ()
{
	double aj,bj,cj,scalar;
	double asum,bsum,csum;
	double epsilon;
	int	j,k;

    /* reproduce initialization */
	aj = 1.0;
	bj = 2.0;
	cj = 0.0;
    /* a[] is modified during timing check */
	aj = 2.0E0 * aj;
    /* now execute timing loop */
	scalar = 3.0;
	for (k=0; k<NTIMES; k++)
        {
            cj = aj;
            bj = scalar*cj;
            cj = aj+bj;
            aj = bj+scalar*cj;
        }
	aj = aj * (double) (N);
	bj = bj * (double) (N);
	cj = cj * (double) (N);

	asum = 0.0;
	bsum = 0.0;
	csum = 0.0;
	for (j=0; j<N; j++) {
		asum += a[j];
		bsum += b[j];
		csum += c[j];
	}
#ifdef VERBOSE
	printf ("Results Comparison: \n");
	printf ("        Expected  : %f %f %f \n",aj,bj,cj);
	printf ("        Observed  : %f %f %f \n",asum,bsum,csum);
#endif

#ifndef abs
#define abs(a) ((a) >= 0 ? (a) : -(a))
#endif
	epsilon = 1.e-8;

	if (abs(aj-asum)/asum > epsilon) {
		printf ("Failed Validation on array a[]\n");
		printf ("        Expected  : %f \n",aj);
		printf ("        Observed  : %f \n",asum);
	}
	else if (abs(bj-bsum)/bsum > epsilon) {
		printf ("Failed Validation on array b[]\n");
		printf ("        Expected  : %f \n",bj);
		printf ("        Observed  : %f \n",bsum);
	}
	else if (abs(cj-csum)/csum > epsilon) {
		printf ("Failed Validation on array c[]\n");
		printf ("        Expected  : %f \n",cj);
		printf ("        Observed  : %f \n",csum);
	}
	else {
		printf ("Solution Validates\n");
	}
}

void copy_task(double *a, double *c, int bs)
{
	int j;	
	for (j=0; j < BSIZE; j++)
		c[j] = a[j];
}

void tuned_STREAM_Copy()
{
	int j;
        for (j=0; j<N; j+=BSIZE) {
//Assumes N is multiple of 100 
            #pragma omp task firstprivate(j)
            copy_task (&a[j], &c[j], BSIZE);
        }
}

void scale_task (double *b, double *c, double scalar, int bs)
{
	int j;	
	for (j=0; j < BSIZE; j++)
	    b[j] = scalar*c[j];
}

void tuned_STREAM_Scale(double scalar)
{
	int j;
//Assumes N is multiple of 100 
	for (j=0; j<N; j+=BSIZE) {
               #pragma omp task firstprivate(j)
	       scale_task (&b[j], &c[j], scalar, BSIZE);
        }
}

void add_task (double *a, double *b, double *c, int bs)
{
	int j;	
	for (j=0; j < BSIZE; j++)
	   c[j] = a[j]+b[j];
}

void tuned_STREAM_Add()
{
	int j;
//Assumes N is multiple of 100 
	for (j=0; j<N; j+=BSIZE) {
            #pragma omp task firstprivate(j)
	    add_task(&a[j], &b[j], &c[j], BSIZE); 
        }
}

void triad_task (double *a, double *b, double *c, double scalar, int bs)
{
	int j;	
	for (j=0; j < BSIZE; j++)
            a[j] = b[j]+scalar*c[j];

}

void tuned_STREAM_Triad(double scalar)
{
	int j;
//Assumes N is multiple of 100 
	for (j=0; j<N; j+=BSIZE) {
            #pragma omp task firstprivate(j)
	    triad_task (&a[j], &b[j], &c[j], scalar, BSIZE);
        }
}
