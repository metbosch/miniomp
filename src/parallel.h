#ifndef __MINIOMP_PARALLEL_H__
#define __MINIOMP_PARALLEL_H__
#include <stdbool.h>
#include "loop.h"
#include "workdescriptor.h"
#include "threadteam.h"

/**
  * Types definition
  */
//typedef struct miniomp_parallel_struct miniomp_parallel_t;

/**
  * Returns the number of threads in the parallel region
  */
unsigned miniomp_parallel_get_num_threads(miniomp_parallel_t *region);

/**
  * Locks the mutex between all threads in the parallel region
  */
void miniomp_parallel_mutex_lock(miniomp_parallel_t *region);

/**
  * Unlocks the mutex between all threads in the parallel region
  */
void miniomp_parallel_mutex_unlock(miniomp_parallel_t *region);

/**
  * Waits on the barrier untill all parallel region team threads calls the function
  * Returns true to one threads, false to others
  */
bool miniomp_parallel_barrier_wait(miniomp_parallel_t *region);

/**
  * Returns a pointer of shared counter to implemente single regions
  */
unsigned *miniomp_parallel_get_single_count(miniomp_parallel_t *region);

/**
  * Returns a pointer to shared loop structure pointer of parallel region
  */
miniomp_loop_t **miniomp_parallel_get_loop(miniomp_parallel_t *region);

/**
  * Returns the work descriptor executed by this parallel region
  */
miniomp_wd_t *miniomp_parallel_get_wd(miniomp_parallel_t *region);

/**
  * Returns a pointer to the thread team executing the parallel region
  */
miniomp_thread_team_t *miniomp_parallel_get_team(miniomp_parallel_t *region);

/**
  * Initializes the global structures for parallel regions creation
  */
void miniomp_parallel_create();

#endif
