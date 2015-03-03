#include "libminiomp.h"
#include "intrinsic.h"
#include "barrier.h"
#include "workdescriptor.h"

// This file handles the PARALLEL construct

// Declaration of array for storing pthread identifier from pthread_create function
//pthread_t *miniomp_threads; // NOT USED

// Global variable for parallel descriptors
//miniomp_parallel_t *miniomp_parallel;
pthread_mutex_t *miniomp_parallel_mutex;
unsigned miniomp_parallel_count;
unsigned miniomp_thread_count;

// declaration of per-thread specific key
pthread_key_t miniomp_specifickey;

// This is the prototype for the Pthreads starting function
void *worker(void *args) {
  // insert all necessary code here for:
  //   1) save thread-specific data
  pthread_setspecific(miniomp_specifickey, args);
  //   2) invoke the per-threads instance of function encapsulating the parallel region
  miniomp_wd_run(&((miniomp_specifickey_ptr)args)->parallel_region->wd);
  //   3) exit the function
  pthread_exit(NULL);
}

miniomp_parallel_t *new_miniomp_parallel_t(void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags, unsigned *first_thread_id) {
  miniomp_parallel_t *ret = (miniomp_parallel_t *)(malloc(sizeof(miniomp_parallel_t)));
 
  ret->num_threads = num_threads;
  ret->threads = (pthread_t *)(malloc(sizeof(pthread_t)*num_threads));
  miniomp_wd_init(&ret->wd, fn, data);
  miniomp_barrier_init(&ret->barrier, num_threads);
 
  CHECK_ERR( pthread_mutex_lock(miniomp_parallel_mutex), 0 );
  ret->id = ++miniomp_parallel_count;
  *first_thread_id = miniomp_thread_count + 1;
  miniomp_thread_count += num_threads - 1;
  /*if (miniomp_parallel == NULL) {
    miniomp_parallel = ret;
  } else {
    miniomp_parallel_t *par = miniomp_parallel;
    while (1) {
      if (par->next == NULL) {
        par->next = ret;
        break;
      } else {
        par = par->next;
      }
    }
  }*/
  CHECK_ERR( pthread_mutex_unlock(miniomp_parallel_mutex), 0 );
 
  return ret;
}

void miniomp_parallel_create_threads(miniomp_parallel_t *region, unsigned first_thread_id) {
  region->threads[0] = pthread_self();
  for (int i = 1; i < region->num_threads; i++) {
    miniomp_specifickey_ptr args = (miniomp_specifickey_ptr)malloc(sizeof(miniomp_specifickey_t));
    args->id = first_thread_id++;
    args->parallel_region = region;
    CHECK_ERR( pthread_create(&region->threads[i], NULL, worker, (void *)args), 0 );
  }
}

void miniomp_parallel_join_threads(miniomp_parallel_t *region) {
  for (int i = 1; i < region->num_threads; i++) {
    CHECK_ERR( pthread_join(region->threads[i], NULL), 0 );
  }
}

void miniomp_parallel_switch_to(miniomp_parallel_t *region) {
  miniomp_specifickey_ptr prev = (miniomp_specifickey_ptr)pthread_getspecific(miniomp_specifickey);
  miniomp_specifickey_t next = { // Same threadID and new parallel region
    id: prev->id,
    parallel_region: region
  };
  pthread_setspecific(miniomp_specifickey, (void *)(&next));
  miniomp_wd_run(&region->wd);
  pthread_setspecific(miniomp_specifickey, (void *)prev);
}

void
GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
  if(!num_threads) num_threads = omp_get_num_threads();
 // printf("Starting a parallel region using %d threads\n", num_threads);
  unsigned first_thread_id;
  miniomp_parallel_t *local_parallel = new_miniomp_parallel_t(fn, data, num_threads, flags, &first_thread_id);
  miniomp_parallel_create_threads(local_parallel, first_thread_id);
  miniomp_parallel_switch_to(local_parallel);
  miniomp_parallel_join_threads(local_parallel);
  free(local_parallel);
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
