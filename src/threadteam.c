#ifndef __MINIOMP_THREAD_TEAM_C__
#define __MINIOMP_THREAD_TEAM_C__
#include <pthread.h>
#include "threadteam.h"
#include "thread.h"
#include "atomicqueue.h"
#include "specifickey.h"
#include "parallel.h"

// Structures definition
struct miniomp_thread_team_struct {
   unsigned num_threads;             // Number of threads in the team
   miniomp_thread_t **threads;        // Thread pointers array
   miniomp_atomic_queue_t queue;     // Queue of tasks to execute by the threads
};

void miniomp_thread_team_init(miniomp_thread_team_t *team, unsigned num_threads) {
   team->num_threads = MAX(num_threads, 1);
   team->threads = (miniomp_thread_t **)(malloc(sizeof(miniomp_thread_t *)*num_threads));
   miniomp_atomic_queue_init(&team->queue);
   for (int i = 0; i < num_threads; ++i) {
      team->threads[i] = new_miniomp_thread_t(i, team);
   }
}

void miniomp_thread_team_destroy(miniomp_thread_team_t *team) {
   miniomp_atomic_queue_destroy(&team->queue);
   for (int i = 0; i < team->num_threads; ++i) {
      destroy_miniomp_thread_t(team->threads[i]);
   }
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

void *worker(void *args) {
   miniomp_set_thread_specifickey((miniomp_specifickey_t *)(args));
   miniomp_parallel_t *region = miniomp_get_parallel_region();
   miniomp_thread_t *thread = miniomp_get_self_thread();
   // Invoke the per-threads instance of function encapsulating the parallel region
   miniomp_wd_run(miniomp_parallel_get_wd(region));
   // Wait work until thread is not running
   while (miniomp_thread_is_running(thread)) {
      miniomp_thread_idle(thread);
      __sync_synchronize();
   }
   // Exit
   miniomp_thread_finish(thread);
   pthread_exit(NULL);
}

miniomp_thread_t *miniomp_thread_team_start(miniomp_thread_team_t *team, miniomp_parallel_t *region) {
   miniomp_thread_set_pthread(team->threads[0], pthread_self());
   for (int i = 1; i < team->num_threads; i++) {
      void *args = (void *)(new_miniomp_specifickey_t(team->threads[i], region));
      miniomp_thread_start(team->threads[0], worker, args);
   }
   return team->threads[0];
}

void miniomp_thread_team_join(miniomp_thread_team_t *team, int self) {
   // Stop all threads
   for (int i = 0; i < team->num_threads; i++) {
      if (i == self) continue;
      miniomp_thread_stop(team->threads[i]);
   }
   __sync_synchronize();
   // Join all threads
   for (int i = 0; i < team->num_threads; i++) {
      if (i == self) continue;
      miniomp_thread_join(team->threads[i]);
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
