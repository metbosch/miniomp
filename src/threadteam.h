#ifndef __MINIOMP_THREAD_TEAM_H__
#define __MINIOMP_THREAD_TEAM_H__
#include <pthread.h>
#include "atomicqueue.h"
#include "libminiomp.h"
#include "workdescriptor.h"
#include "parallel.h"

//typedef struct miniomp_thread_struct miniomp_thread_t;

//typedef struct miniomp_thread_team_struct miniomp_thread_team_t;

/*typedef struct {
  pthread_t pthread;
  bool running;
} miniomp_thread_t;*/

/*typedef struct {
  unsigned num_threads;
  miniomp_thread_t *threads;
  miniomp_atomic_queue_t queue;
} miniomp_thread_team_t;*/

void miniomp_thread_team_init(miniomp_thread_team_t *team, unsigned num_threads);

void miniomp_thread_team_destroy(miniomp_thread_team_t *team);

miniomp_thread_team_t *new_miniomp_thread_team_t(unsigned num_threads);

void destroy_miniomp_thread_team_t(miniomp_thread_team_t *team);

void miniomp_thread_team_start(miniomp_thread_team_t *team, miniomp_parallel_t *region);

void miniomp_thread_team_join(miniomp_thread_team_t *team, int self);

unsigned miniomp_thread_team_get_num_threads(miniomp_thread_team_t *team);

void miniomp_thread_team_push_task(miniomp_thread_team_t *team, miniomp_wd_t *task);

miniomp_wd_t *miniomp_thread_team_pop_task(miniomp_thread_team_t *team);

bool miniomp_thread_team_taskwait(miniomp_thread_team_t *team);

#endif
