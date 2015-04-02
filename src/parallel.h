#ifndef __MINIOMP_PARALLEL_H__
#define __MINIOMP_PARALLEL_H__
#include <stdbool.h>
#include "loop.h"
#include "workdescriptor.h"
#include "threadteam.h"

//typedef struct miniomp_parallel_struct miniomp_parallel_t;

unsigned miniomp_parallel_get_num_threads(miniomp_parallel_t *region);

void miniomp_parallel_mutex_lock(miniomp_parallel_t *region);

void miniomp_parallel_mutex_unlock(miniomp_parallel_t *region);

bool miniomp_parallel_barrier_wait(miniomp_parallel_t *region);

unsigned *miniomp_parallel_get_single_count(miniomp_parallel_t *region);

miniomp_loop_t **miniomp_parallel_get_loop(miniomp_parallel_t *region);

miniomp_wd_t *miniomp_parallel_get_wd(miniomp_parallel_t *region);

miniomp_thread_team_t *miniomp_parallel_get_team(miniomp_parallel_t *region);

void miniomp_parallel_create();

#endif
