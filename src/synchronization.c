#include "libminiomp.h"
#include "intrinsic.h"
#include <pthread.h>

// Default lock for critical sections
pthread_mutex_t miniomp_default_lock;

void GOMP_critical_start (void) {
  printf("TBI: Entering an unnamed critical, don't know if anyone else is alrady in. I proceed\n");
}

void GOMP_critical_end () {
  printf("Exiting an unnamed critical section. I can not inform anyone else\n");
}

#if MYBARRIER
mybarrier_t mybarrier;
#else
pthread_barrier_t miniomp_barrier;
#endif

void GOMP_barrier() {
  printf("TBI: Entering in barrier, but do not know how to wait for the rest. I proceed\n");
}
