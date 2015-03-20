#include <unistd.h>
#include "libminiomp.h"
#include "intrinsic.h"
#include "specifickey.h"

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

int miniomp_debug_atomic = 0;
void DEBUG(char *str) {
  if (miniomp_icv.debug_enabled) {
    while (!(__sync_bool_compare_and_swap(&miniomp_debug_atomic, 0, 1)));
    printf("%d:\t", miniomp_get_thread_specifickey()->id);
    printf(str);
    printf("\n");
    miniomp_debug_atomic = 0;
  }
}

void init_pthread_structs(void) {
  miniomp_parallel_count = 0;
  miniomp_thread_count = 1;
  // Init mutex for modify the parallel regions list
  miniomp_parallel_mutex = (pthread_mutex_t *)(malloc(sizeof(pthread_mutex_t)));
  CHECK_ERR( pthread_mutex_init(miniomp_parallel_mutex, NULL), 0 );
}

void fini_pthread_structs(void) {
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
  init_pthread_structs();
  miniomp_specifickey_create();
  miniomp_set_thread_specifickey(new_miniomp_specifickey_t(0, NULL));
  // Initialize OpenMP default lock and default barrier
  // Initialize OpenMP workdescriptors for loop and single and taskqueue
}

void
fini_miniomp(void) {
  // free structures allocated during library initialization
  fini_pthread_structs();
  printf ("mini-omp is finalized\n");
}
