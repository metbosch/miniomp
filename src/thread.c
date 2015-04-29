#ifndef __MINIOMP_THREAD_C__
#define __MINIOMP_THREAD_C__
#include "thread.h"
#include "specifickey.h"

// Structures definition
typedef struct {
   bool running;
   bool must_stop;
   bool has_joined;
} miniomp_thread_status_t;

struct miniomp_thread_struct {
   unsigned id;                      // Thread identifier in the team
   pthread_t pthread;                // Pthread information
   miniomp_thread_status_t status;   // Statused of thread
   miniomp_wd_t *wd;                 // Executing workdescriptor
   miniomp_thread_team_t *team;      // Thread team
};

void miniomp_thread_init(miniomp_thread_t *thread, unsigned id, miniomp_thread_team_t *team) {
   thread->id = id;
   thread->status.running = false;
   thread->status.must_stop = false;
   thread->status.has_joined = false;
   thread->wd = NULL;
   thread->team = team;
}

miniomp_thread_t *new_miniomp_thread_t(unsigned id, miniomp_thread_team_t *team) {
   miniomp_thread_t *ret = (miniomp_thread_t *)(malloc(sizeof(miniomp_thread_t)));
   miniomp_thread_init(ret, id, team);
   return ret;
}

void destroy_miniomp_thread_t(miniomp_thread_t *thread) {
   free(thread);
}

inline void miniomp_thread_set_wd(miniomp_thread_t *thread, miniomp_wd_t *wd) {
   thread->status.running = true;
   thread->wd = wd;
}

inline miniomp_wd_t *miniomp_thread_get_wd(miniomp_thread_t *thread) {
   return thread->wd;
}

inline unsigned miniomp_thread_get_id(miniomp_thread_t *thread) {
   return thread->id;
}

inline void miniomp_thread_stop(miniomp_thread_t *thread) {
   thread->status.must_stop = true;
}

inline bool miniomp_thread_is_running(miniomp_thread_t *thread) {
   return thread->status.running && !thread->status.must_stop;
}

inline void miniomp_thread_set_pthread(miniomp_thread_t *thread, pthread_t info) {
   thread->pthread = info;
   thread->status.running = true;
}

void miniomp_thread_start(miniomp_thread_t *thread, void * (*worker_fn) (void *), void *args) {
   CHECK_ERR( pthread_create(&(thread->pthread), NULL, worker_fn, args), 0 );
   thread->status.running = true;
}

void miniomp_thread_finish(miniomp_thread_t *thread) {
   thread->status.has_joined = true;
   __sync_synchronize();
}

void miniomp_thread_join(miniomp_thread_t *thread) {
//   CHECK_ERR( pthread_join(thread->pthread, NULL), 0 );
   while(!thread->status.has_joined) {
      __sync_synchronize();
   }
}

void miniomp_thread_taskwait(miniomp_thread_t *thread) {
   miniomp_wd_t *waiting = thread->wd;
   if (waiting == NULL) return;
   while (miniomp_wd_has_components(waiting)) {
      miniomp_wd_t *work = miniomp_thread_team_pop_task(thread->team);
      if (work != NULL) {
         miniomp_wd_run(work);
      }
      // Can be no work to execute but work executing, so wait
   }
}

void miniomp_thread_idle(miniomp_thread_t *thread) {
   miniomp_wd_t *work = miniomp_thread_team_pop_task(thread->team);
   while(work != NULL) {
      miniomp_wd_run(work);
      work = miniomp_thread_team_pop_task(thread->team);
   }
}

void miniomp_thread_blocked(miniomp_thread_t *thread) {
   miniomp_wd_t *work = miniomp_thread_team_pop_task(thread->team);
   if (work != NULL) {
      miniomp_wd_run(work);
   }
}

#endif
