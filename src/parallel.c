#ifndef __MINIOMP_PARALLEL_C__
#define __MINIOMP_PARALLEL_C__
#include "parallel.h"
#include "libminiomp.h"
#include "intrinsic.h"
#include "barrier.h"
#include "workdescriptor.h"
#include "specifickey.h"
#include "threadteam.h"
#include "loop.h"
#include "thread.h"

// Struct definition
struct miniomp_parallel_struct {
   unsigned id;
   miniomp_wd_t wd;
   miniomp_thread_team_t* team; // Team of threads for this region
   miniomp_barrier_t barrier;   // Barrier for this parallel region
   pthread_mutex_t mutex;       // Mutex for critical regions
   unsigned single_count;       // Counter for single regions
   miniomp_loop_t *loop;        // Pointer to parallel loop structure
};

// Global variable for parallel regions count
unsigned miniomp_parallel_count;

miniomp_parallel_t *new_miniomp_parallel_t(void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags) {
   miniomp_parallel_t *ret = (miniomp_parallel_t *)(malloc(sizeof(miniomp_parallel_t)));

   ret->single_count = 0;
   ret->loop = NULL;
   ret->team = new_miniomp_thread_team_t(num_threads);
   miniomp_wd_init(&ret->wd, fn, data, NULL, PARALLEL);
   miniomp_barrier_init(&ret->barrier, num_threads);
   CHECK_ERR( pthread_mutex_init(&ret->mutex, NULL), 0 );
   ret->id = __sync_fetch_and_add(&miniomp_parallel_count, 1);

   return ret;
}

void destroy_miniomp_parallel_t(miniomp_parallel_t *region) {
   destroy_miniomp_thread_team_t(region->team);
   miniomp_barrier_destroy(&region->barrier);
   CHECK_ERR( pthread_mutex_destroy(&region->mutex), 0 );
   free(region);
}

void miniomp_parallel_join_threads(miniomp_parallel_t *region) {
   miniomp_thread_team_join(region->team, 0);
}
unsigned miniomp_parallel_get_num_threads(miniomp_parallel_t *region) {
   return (miniomp_thread_team_get_num_threads(region->team));
}

void miniomp_parallel_mutex_lock(miniomp_parallel_t *region) {
   CHECK_ERR( pthread_mutex_lock(&region->mutex), 0 );
}

void miniomp_parallel_mutex_unlock(miniomp_parallel_t *region) {
   CHECK_ERR( pthread_mutex_unlock(&region->mutex), 0 );
}

bool miniomp_parallel_barrier_wait(miniomp_parallel_t *region) {
   return miniomp_barrier_wait(&region->barrier);
}

unsigned *miniomp_parallel_get_single_count(miniomp_parallel_t *region) {
   return &region->single_count;
}

miniomp_loop_t **miniomp_parallel_get_loop(miniomp_parallel_t *region) {
   return &region->loop;
}

miniomp_wd_t *miniomp_parallel_get_wd(miniomp_parallel_t *region) {
   return &region->wd;
}

miniomp_thread_team_t *miniomp_parallel_get_team(miniomp_parallel_t *region) {
   return region->team;
}

void miniomp_parallel_create() {
   miniomp_parallel_count = 0;
}

void
GOMP_parallel (void (*fn) (void *), void *data, unsigned num_threads, unsigned int flags)
{
   if(!num_threads) num_threads = miniomp_icv.nthreads_var;
   DEBUG("Starting new parallel region");
   miniomp_parallel_t *local_parallel = new_miniomp_parallel_t(fn, data, num_threads, flags);
   miniomp_thread_t *self = miniomp_thread_team_start(local_parallel->team, local_parallel);
   miniomp_specifickey_t *next = new_miniomp_specifickey_t(self, local_parallel);
   miniomp_set_thread_specifickey(next);

   miniomp_wd_run(&local_parallel->wd);
   miniomp_parallel_join_threads(local_parallel);

   DEBUG("Exiting parallel region");
   miniomp_pop_thread_specifickey();
   destroy_miniomp_specifickey_t(next);
   destroy_miniomp_parallel_t(local_parallel);
}

void
GOMP_parallel_start (void (*fn) (void *), void *data, unsigned num_threads)
{
   if(!num_threads) num_threads = miniomp_icv.nthreads_var;
   DEBUG("Starting new parallel region");
   miniomp_parallel_t *local_parallel = new_miniomp_parallel_t(fn, data, num_threads, 0);
   miniomp_thread_t *self = miniomp_thread_team_start(local_parallel->team, local_parallel);
   miniomp_specifickey_t *next = new_miniomp_specifickey_t(self, local_parallel);
   miniomp_set_thread_specifickey(next);
}

void
GOMP_parallel_end (void)
{
   miniomp_parallel_join_threads(miniomp_get_parallel_region());

   DEBUG("Exiting parallel region");
   miniomp_specifickey_t *key = miniomp_pop_thread_specifickey();
   destroy_miniomp_parallel_t(miniomp_get_parallel_region(key));
   destroy_miniomp_specifickey_t(key);
}

#endif
