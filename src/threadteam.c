#ifndef __MINIOMP_THREAD_TEAM_C__
#define __MINIOMP_THREAD_TEAM_C__
#include <pthread.h>
#include "threadteam.h"
#include "atomicqueue.h"
#include "specifickey.h"
#include "parallel.h"

// Structures definition
/*struct miniomp_thread_struct {
  pthread_t pthread;
}*/;

struct miniomp_thread_team_struct {
  unsigned num_threads;
  miniomp_thread_t *threads;
  miniomp_atomic_queue_t queue;
};

void miniomp_thread_team_init(miniomp_thread_team_t *team, unsigned num_threads) {
  team->num_threads = num_threads;
  team->threads = (miniomp_thread_t *)(malloc(sizeof(miniomp_thread_t)*num_threads));
  miniomp_atomic_queue_init(&team->queue);
}

void miniomp_thread_team_destroy(miniomp_thread_team_t *team) {
  miniomp_atomic_queue_destroy(&team->queue);
  free(team->threads);
}

miniomp_thread_team_t *new_miniomp_thread_team_t(unsigned num_threads) {
  miniomp_thread_team_t *ret = (miniomp_thread_team_t *)(malloc(sizeof(miniomp_thread_team_t)));
  miniomp_thread_team_init(ret, num_threads);
  return ret;
}

void destroy_miniomp_thread_team_t(miniomp_thread_team_t *team) {
  miniomp_thread_team_destroy(team);
  free(team);
  //team = NULL;
}

// This is the prototype for the Pthreads starting function
void *worker(void *args) {
  // insert all necessary code here for:
  //   1) save thread-specific data
  miniomp_set_thread_specifickey((miniomp_specifickey_t *)(args));
  //printf("Starting thread num %d\n", omp_get_thread_num());
  //   2) invoke the per-threads instance of function encapsulating the parallel region
  miniomp_wd_run(miniomp_parallel_get_wd(((miniomp_specifickey_t *)args)->parallel_region));
  //   3) exit the function
  pthread_exit(NULL);
}

void miniomp_thread_team_start(miniomp_thread_team_t *team, miniomp_parallel_t *region) {
  team->threads[0].pthread = pthread_self();
  for (int i = 1; i < team->num_threads; i++) {
    miniomp_specifickey_t *args = new_miniomp_specifickey_t(i, region);
    CHECK_ERR( pthread_create(&team->threads[i].pthread, NULL, worker, (void *)args), 0 );
  }
}

void miniomp_thread_team_join(miniomp_thread_team_t *team, int self) {
  for (int i = 0; i < team->num_threads; i++) {
    if (i == self) continue;
    CHECK_ERR( pthread_join(team->threads[i].pthread, NULL), 0 );
  }
}

unsigned miniomp_thread_team_get_num_threads(miniomp_thread_team_t *team) {
  return team->num_threads;
}

void miniomp_thread_team_push_task(miniomp_thread_team_t *team, miniomp_wd_t *task) {
  miniomp_atomic_queue_push(&team->queue, (void *)(task));
}

miniomp_wd_t *miniomp_thread_team_pop_task(miniomp_thread_team_t *team) {
  return miniomp_atomic_queue_pop(&team->queue);
}

#endif
