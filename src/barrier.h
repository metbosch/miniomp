#ifndef __MINIOMP_BARRIER__
#define __MINIOMP_BARRIER__
#include <pthread.h>
#include <stdlib.h>

#define MYBARRIER 0
#if MYBARRIER
//  Here you should define all neceesary types and variables to implement your barrier
typedef struct {
    // complete the definition of barrier
} miniomp_barrier_t;
#else
typedef pthread_barrier_t miniomp_barrier_t;
#endif

void CHECK_ERR(int result, int expected);

// Barrier related functions
void miniomp_barrier_init(miniomp_barrier_t *restrict barrier, unsigned count);
void miniomp_barrier_destroy(miniomp_barrier_t *barrier);
void miniomp_barrier_wait(miniomp_barrier_t *barrier);

#endif
