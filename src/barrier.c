#include "barrier.h"

void init_pthread_barrier(pthread_barrier_t *barrier, unsigned count) {
  CHECK_ERR( pthread_barrier_init(barrier, NULL, count), 0 );
}

void destroy_pthread_barrier(pthread_barrier_t *barrier) {
  CHECK_ERR( pthread_barrier_destroy(barrier), 0 );
}

void wait_pthread_barrier(pthread_barrier_t *barrier) {
  int ret = pthread_barrier_wait(barrier);
  if (ret != 0 && ret != PTHREAD_BARRIER_SERIAL_THREAD) {
    exit(ret);
  }
}

void init_custom_barrier(miniomp_barrier_t *barrier, unsigned count) {
}

void destroy_custom_barrier(miniomp_barrier_t *barrier) {
}

void wait_barrier_wait(miniomp_barrier_t *barrier) {
}

void miniomp_barrier_init(miniomp_barrier_t *restrict barrier, unsigned count) {
  init_pthread_barrier((pthread_barrier_t *)barrier, count);
}

void miniomp_barrier_destroy(miniomp_barrier_t *barrier) {
  destroy_pthread_barrier((pthread_barrier_t *)barrier);
}

void miniomp_barrier_wait(miniomp_barrier_t *barrier) {
  wait_pthread_barrier((pthread_barrier_t *)barrier);
}

