#include "libminiomp.h"
#include "intrinsic.h"

// This file handles the PARALLEL construct

// Declaration of array for storing pthread identifier from pthread_create function
pthread_t *miniomp_threads;

// Global variable for parallel descriptor
miniomp_parallel_t *miniomp_parallel;

// declaration of per-thread specific key
pthread_key_t miniomp_specifickey;

// This is the prototype for the Pthreads starting function
void *worker(void *args) {
  // insert all necessary code here for:
  //   1) save thread-specific data
  //   2) invoke the per-threads instance of function encapsulating the parallel region
  //   3) exit the function
}

void
GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
  if(!num_threads) num_threads = omp_get_num_threads();
  printf("Starting a parallel region using %d threads\n", num_threads);
  miniomp_parallel_t *local_parallel = (miniomp_parallel_t *)malloc(sizeof(miniomp_parallel_t));
  // TODO: add to global parallel the new region
  local_parallel->threads = miniomp_threads;
  local_parallel->fn_data = data;
  local_parallel->fn = fn;
  local_parallel->id = 1; //Now only supporting one parallel region
  for (int i=1; i<num_threads; i++) {
      //fn (data);
    CHECK_ERR( pthread_create(local_parallel->threads[i], NULL, worker, NULL), 0 );
  }
}

// When using the following splitted interface, the master invokes foo after returning from GOMP_parallel_start
void
GOMP_parallel_start (void (*fn) (void *), void *data, unsigned num_threads) {
  if(!num_threads) num_threads = omp_get_num_threads();
  printf("Starting a parallel region using %d threads\n", num_threads);
  for (int i=0; i<num_threads-1; i++)
      fn (data);
}

void
GOMP_parallel_end (void) {
  printf("Reaching the end of the parallel region\n");
}
