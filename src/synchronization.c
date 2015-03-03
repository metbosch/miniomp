#include "libminiomp.h"
#include "intrinsic.h"
#include <pthread.h>

void GOMP_critical_start (void) {
   // printf("Critical_start begin\n");
    miniomp_specifickey_ptr info = (miniomp_specifickey_ptr)(pthread_getspecific(miniomp_specifickey));
    CHECK_ERR( pthread_mutex_lock(&info->parallel_region->mutex), 0 );
   // printf("Critical_start end\n");
}

void GOMP_critical_end () {
    miniomp_specifickey_ptr info = (miniomp_specifickey_ptr)(pthread_getspecific(miniomp_specifickey));
    CHECK_ERR( pthread_mutex_unlock(&info->parallel_region->mutex), 0 );

}

void GOMP_barrier() {
    miniomp_specifickey_ptr info = (miniomp_specifickey_ptr)(pthread_getspecific(miniomp_specifickey));
   // printf("%d\tBarrier begin\n", info->id);
    miniomp_barrier_wait(&info->parallel_region->barrier);
   // printf("%d\tBarrier end\n", info->id);
}
