#include "libminiomp.h"
#include "intrinsic.h"
#include "barrier.h"
#include "workdescriptor.h"
#include "specifickey.h"
#include "threadteam.h"

// This file handles the PARALLEL construct

// Declaration of array for storing pthread identifier from pthread_create function
//pthread_t *miniomp_threads; // NOT USED

// Global variable for parallel descriptors
//miniomp_parallel_t *miniomp_parallel;
pthread_mutex_t *miniomp_parallel_mutex;
unsigned miniomp_parallel_count;
unsigned miniomp_thread_count;

miniomp_parallel_t *new_miniomp_parallel_t(void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
  miniomp_parallel_t *ret = (miniomp_parallel_t *)(malloc(sizeof(miniomp_parallel_t)));
 
  ret->single_count = 0;
  ret->loop = NULL;
  miniomp_thread_team_init(&ret->team, num_threads)
  miniomp_wd_init(&ret->wd, fn, data);
  miniomp_barrier_init(&ret->barrier, num_threads);
  CHECK_ERR( pthread_mutex_init(&ret->mutex, NULL), 0 );
  //CHECK_ERR( pthread_mutex_init(&ret->loop_mutex, NULL), 0 );

  CHECK_ERR( pthread_mutex_lock(miniomp_parallel_mutex), 0 );
  ret->id = ++miniomp_parallel_count;
  //*first_thread_id = miniomp_thread_count;
  //miniomp_thread_count += num_threads - 1;
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

void destroy_miniomp_parallel_t(miniomp_parallel_t *region) {
  miniomp_thread_team_destroy(&region->team);
  miniomp_barrier_destroy(&region->barrier);
  CHECK_ERR( pthread_mutex_destroy(&region->mutex), 0 );
  free(region);
}

void miniomp_parallel_create_threads(miniomp_parallel_t *region) {
  miniomp_thread_team_start(&region->team, region);
}

void miniomp_parallel_join_threads(miniomp_parallel_t *region) {
  miniomp_thread_team_join(&region->team, 0);
}

void miniomp_parallel_switch_to(miniomp_parallel_t *region) {
  miniomp_specifickey_t *prev = miniomp_get_thread_specifickey();
  miniomp_specifickey_t *next = new_miniomp_specifickey_t(0, region);
  miniomp_set_thread_specifickey(next);
  miniomp_wd_run(&region->wd);
  miniomp_set_thread_specifickey(prev);
  destroy_miniomp_specifickey_t(next);
}

void
GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
  if(!num_threads) num_threads = miniomp_icv.nthreads_var;
 // printf("Starting a parallel region using %d threads\n", num_threads);
  unsigned first_thread_id;
  miniomp_parallel_t *local_parallel = new_miniomp_parallel_t(fn, data, num_threads, flags);
  miniomp_parallel_create_threads(local_parallel);
  miniomp_parallel_switch_to(local_parallel);
  miniomp_parallel_join_threads(local_parallel);
  destroy_miniomp_parallel_t(local_parallel);
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
