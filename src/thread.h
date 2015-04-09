#ifndef __MINIOMP_THREAD_H__
#define __MINIOMP_THREAD_H__
#include <pthread.h>
#include "libminiomp.h"
#include "workdescriptor.h"
#include "threadteam.h"

/**
  * Types definition
  */
//typedef struct miniomp_thread_struct miniomp_thread_t;

/**
  * Initializes an existing thread object
  */
void miniomp_thread_init(miniomp_thread_t *thread, unsigned id, miniomp_thread_team_t *team);

/**
  * Creates a new object with specified information
  */
miniomp_thread_t *new_miniomp_thread_t(unsigned id, miniomp_thread_team_t *team);

/**
  * Destroys an existing object
  */
void destroy_miniomp_thread_t(miniomp_thread_t *thread);

/**
  * Getter of thread identifier
  */
unsigned miniomp_thread_get_id(miniomp_thread_t *thread);

/**
  * Setter of executing workdescriptor by thread
  */
void miniomp_thread_set_wd(miniomp_thread_t *thread, miniomp_wd_t *wd);

/**
  * Getter of executing workdescriptor for specified thread
  */
miniomp_wd_t *miniomp_thread_get_wd(miniomp_thread_t *thread);

/**
  * Stops the thread execution
  */
void miniomp_thread_stop(miniomp_thread_t *thread);

/**
  * Marks the thread as finished and ready to join
  */
void miniomp_thread_finish(miniomp_thread_t *thread);

/**
  * Setter of pthread info and mark as running
  */
void miniomp_thread_set_pthread(miniomp_thread_t *thread, pthread_t info);

/**
  * Returns true if the thread is running, false otherwise
  */
bool miniomp_thread_is_running(miniomp_thread_t *thread);

/**
  * Starts the thread
  */
void miniomp_thread_start(miniomp_thread_t *thread, void * (*worker_fn)(void *), void *args);

/**
  * Waits until the thread ends
  */
void miniomp_thread_join(miniomp_thread_t *thread);

/**
  * Waits untill all descendents tasks of current thread workdescriptor are completed
  */
void miniomp_thread_taskwait(miniomp_thread_t *thread);

/**
  * Marks the thread as idle and waits until must stop
  */
void miniomp_thread_idle(miniomp_thread_t *thread);

/**
  * Marks the thread as blocked and checks if can do some team work
  */
void miniomp_thread_blocked(miniomp_thread_t *thread);

#endif
