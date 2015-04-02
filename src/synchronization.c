#include "libminiomp.h"
#include "intrinsic.h"
#include <pthread.h>
#include "specifickey.h"
#include "parallel.h"

void GOMP_critical_start (void) {
  // printf("Critical_start begin\n");
  // CHECK_ERR( pthread_mutex_lock(&(miniomp_get_thread_specifickey()->parallel_region->mutex)), 0 );
  miniomp_parallel_mutex_lock(miniomp_get_parallel_region());
  // printf("Critical_start end\n");
}

void GOMP_critical_end () {
  miniomp_parallel_mutex_unlock(miniomp_get_parallel_region());
  // CHECK_ERR( pthread_mutex_unlock(&(miniomp_get_thread_specifickey()->parallel_region->mutex)), 0 );
}

void GOMP_barrier() {
  miniomp_parallel_barrier_wait(miniomp_get_parallel_region());
  //miniomp_barrier_wait(&(miniomp_get_thread_specifickey()->parallel_region->barrier));
}
