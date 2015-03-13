#ifndef __MINIOMP_THREAD_TEAM_H__
#define __MINIOMP_THREAD_TEAM_H__
#include <pthread.h>
#include "atomicqueue.h"
#include "libminiomp.h"

typedef struct {
  pthread_t pthread;
} miniomp_thread_t;

typedef struct {
  unsigned num_threads;
  miniomp_thread_t *threads;
  miniomp_atomic_queue_t queue;
} miniomp_thread_team_t;

void miniomp_thread_team_init(miniomp_thread_team_t *team, unsigned num_threads);

void miniomp_thread_team_destroy(miniomp_thread_team_t *team);

void miniomp_thread_team_start(miniomp_thread_team_t *team, miniomp_parallel_t *region);

void miniomp_thread_team_join(miniomp_thread_team_t *team, int self);

void miniomp_push_task(miniomp_thread_team_t *team, miniomp_wd_t *task);

#endif
