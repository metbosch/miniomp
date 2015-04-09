#ifndef __MINIOMP_THREAD_TEAM_H__
#define __MINIOMP_THREAD_TEAM_H__
#include "libminiomp.h"
#include "workdescriptor.h"
#include "parallel.h"

/**
  * Types definition
  */
//typedef struct miniomp_thread_team_struct miniomp_thread_team_t;

/**
  * Initializes an existing thread team object
  */
void miniomp_thread_team_init(miniomp_thread_team_t *team, unsigned num_threads);

/**
  * Frees and uninitializes an existing thread team object
  */
void miniomp_thread_team_destroy(miniomp_thread_team_t *team);

/**
  * Creates a new object with the specified information
  */
miniomp_thread_team_t *new_miniomp_thread_team_t(unsigned num_threads);

/**
  * Destroys an existing object
  */
void destroy_miniomp_thread_team_t(miniomp_thread_team_t *team);

/**
  * Starts the threads in the team
  * Returns pointer to caller info thread
  */
miniomp_thread_t * miniomp_thread_team_start(miniomp_thread_team_t *team, miniomp_parallel_t *region);

/**
  * Stops and joins all threads in the teams (Doesn't stops the thread identified by self)
  */
void miniomp_thread_team_join(miniomp_thread_team_t *team, int self);

/**
  * Returns the number of threads in the team
  */
unsigned miniomp_thread_team_get_num_threads(miniomp_thread_team_t *team);

/**
  * Pushes a new task to be fone by the team threads
  */
void miniomp_thread_team_push_task(miniomp_thread_team_t *team, miniomp_wd_t *task);

/**
  * Returns a task to execute by one thread team
  * Retuns NULL if no one is available
  */
miniomp_wd_t *miniomp_thread_team_pop_task(miniomp_thread_team_t *team);

#endif
