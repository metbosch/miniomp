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

void specifickey_destroy(void * arg) {
  miniomp_specifickey_ptr mine = (miniomp_specifickey_ptr) arg; // deallocate data
  free (mine);
}

void init_pthread(void) {
  miniomp_parallel_count = 0;
  miniomp_thread_count = 1;
  // Init mutex for modify the parallel regions list
  miniomp_parallel_mutex = (pthread_mutex_t *)(malloc(sizeof(pthread_mutex_t)));
  CHECK_ERR( pthread_mutex_init(miniomp_parallel_mutex, NULL), 0 );

  // Init per-thread structures
  CHECK_ERR( pthread_key_create(&miniomp_specifickey, specifickey_destroy), 0 );
  miniomp_specifickey_ptr mine = (miniomp_specifickey_ptr) malloc (sizeof (miniomp_specifickey_t));
  mine->id = 1;
  pthread_setspecific(miniomp_specifickey, (void *) mine);
}

void fini_pthread(void) {
  // Parallel regions list mutex
  CHECK_ERR( pthread_mutex_destroy(miniomp_parallel_mutex), 0 );
  free((void *)(miniomp_parallel_mutex));
}

void
init_miniomp(void) {
  printf ("mini-omp is being initialized\n");
  // Parse OMP_NUM_THREADS environment variable to initialize nthreads_var internal control variable
  parse_env();
  // Initialize Pthread data structures and thread-specific data
  init_pthread();
  // Initialize OpenMP default lock and default barrier
  // Initialize OpenMP workdescriptors for loop and single and taskqueue
}

void
fini_miniomp(void) {
  // free structures allocated during library initialization
  fini_pthread();
  printf ("mini-omp is finalized\n");
}
