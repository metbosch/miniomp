#include <unistd.h>
#include "libminiomp.h"
#include "intrinsic.h"

// Library constructor and desctructor
void init_miniomp(void) __attribute__((constructor));
void fini_miniomp(void) __attribute__((destructor));

// Function to parse OMP_NUM_THREADS environment variable
void parse_env(void);

void CHECK_ERR(int result, int expected) {
  if (result != expected) {
    printf("ERROR DETECTED:\texpected return code: %d, returned: %d\n", expected, result);
    exit(result);
  }
}

void init_pthread_barrier(void) {
  CHECK_ERR( pthread_barrier_init(&miniomp_barrier, NULL, miniomp_icv.nthreads_var), 0 );
}

void fini_pthread_barrier(void) {
  CHECK_ERR( pthread_barrier_destroy(&miniomp_barrier), 0 );
}

void init_barrier(void) {
  init_pthread_barrier();
}

void fini_barrier(void) {
  fini_pthread_barrier();
}

void
init_miniomp(void) {
  printf ("mini-omp is being initialized\n");
  // Parse OMP_NUM_THREADS environment variable to initialize nthreads_var internal control variable
  parse_env();
  // Initialize Pthread data structures and thread-specific data
  // Initialize OpenMP default lock and default barrier
  init_barrier();
  // Initialize OpenMP workdescriptors for loop and single and taskqueue
}

void
fini_miniomp(void) {
  // free structures allocated during library initialization
  fini_barrier();
  printf ("mini-omp is finalized\n");
}
