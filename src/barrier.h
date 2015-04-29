#ifndef __MINIOMP_BARRIER_H__
#define __MINIOMP_BARRIER_H__
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

#define MYBARRIER 1

/**
  * Structures and types definition
  */
#if MYBARRIER
typedef struct {
   unsigned count;        // Number od threads in the barrier
   unsigned target;       // Number of expected threads in the barrier
   unsigned char flag;    // Waiting condition
   pthread_mutex_t mutex; // Barrier mutex
} miniomp_barrier_t;
#else
typedef pthread_barrier_t miniomp_barrier_t;
#endif

/**
  * Initializes an existing barrier object
  */
void miniomp_barrier_init(miniomp_barrier_t *restrict barrier, unsigned count);

/**
  * Frees and uninitializes an existing barrier object
  */
void miniomp_barrier_destroy(miniomp_barrier_t *barrier);

/**
  * Waits on the barrier until target threads are waiting
  * Returns true to one threads, false to others
  */
bool miniomp_barrier_wait(miniomp_barrier_t *barrier);

#endif
