#include "libminiomp.h"
#include "intrinsic.h"
#include <pthread.h>
#include "specifickey.h"

void GOMP_critical_start (void) {
   // printf("Critical_start begin\n");
    CHECK_ERR( pthread_mutex_lock(&(miniomp_get_thread_specifickey()->parallel_region->mutex)), 0 );
   // printf("Critical_start end\n");
}

void GOMP_critical_end () {
    CHECK_ERR( pthread_mutex_unlock(&(miniomp_get_thread_specifickey()->parallel_region->mutex)), 0 );
}

void GOMP_barrier() {
    miniomp_barrier_wait(&(miniomp_get_thread_specifickey()->parallel_region->barrier));
}
