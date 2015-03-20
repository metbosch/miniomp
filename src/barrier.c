#include "barrier.h"
#include "libminiomp.h"

#if !MYBARRIER
void init_pthread_barrier(pthread_barrier_t *barrier, unsigned count) {
  CHECK_ERR( pthread_barrier_init(barrier, NULL, count), 0 );
}
#endif

#if !MYBARRIER
void destroy_pthread_barrier(pthread_barrier_t *barrier) {
  CHECK_ERR( pthread_barrier_destroy(barrier), 0 );
}
#endif

#if !MYBARRIER
bool wait_pthread_barrier(pthread_barrier_t *barrier) {
  int ret = pthread_barrier_wait(barrier);
  if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
    exit(ret);
  }
  return (ret == PTHREAD_BARRIER_SERIAL_THREAD);
}
#endif

#if MYBARRIER
void init_custom_barrier(miniomp_barrier_t *barrier, unsigned count) {
  barrier->target = count;
  barrier->count = 0;
  barrier->flag = 0;
  CHECK_ERR( pthread_mutex_init(&barrier->mutex, NULL), 0 );
}
#endif

#if MYBARRIER
void destroy_custom_barrier(miniomp_barrier_t *barrier) {
  if (barrier->count) {
    printf("ERROR: Trying to destroy a barrier with %d waiters\n", barrier->count);
  }
  CHECK_ERR( pthread_mutex_destroy(&barrier->mutex), 0 );
}
#endif

#if MYBARRIER
bool wait_custom_barrier(miniomp_barrier_t *barrier) {
  DEBUG("Entering in a barrier");
  CHECK_ERR( pthread_mutex_lock(&barrier->mutex), 0 );
  barrier->count++;
  unsigned char local_flag = barrier->flag;
  if (barrier->count == barrier->target) {
    DEBUG ("Flag up!");
    barrier->count = 0;
    barrier->flag ^= 1;
    __sync_synchronize();
    CHECK_ERR( pthread_mutex_unlock(&barrier->mutex), 0 );
    return true;
  }
  CHECK_ERR( pthread_mutex_unlock(&barrier->mutex), 0 );
  DEBUG("Waiting barrier");
  while(barrier->flag == local_flag) {
     __sync_synchronize();
  }
  DEBUG("Leaving barrier");
  return false;
}
#endif

void miniomp_barrier_init(miniomp_barrier_t *restrict barrier, unsigned count) {
#if MYBARRIER
  init_custom_barrier(barrier, count);
#else
  init_pthread_barrier((pthread_barrier_t *)barrier, count);
#endif
}

void miniomp_barrier_destroy(miniomp_barrier_t *barrier) {
#if MYBARRIER
  destroy_custom_barrier(barrier);
#else
  destroy_pthread_barrier((pthread_barrier_t *)barrier);
#endif
}

bool miniomp_barrier_wait(miniomp_barrier_t *barrier) {
#if MYBARRIER
  return wait_custom_barrier(barrier);
#else
  return wait_pthread_barrier((pthread_barrier_t *)barrier);
#endif
}

