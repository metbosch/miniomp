#ifndef __MINIOMP_LIB_H__
#define __MINIOMP_LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "parallel.h"

#define MAX_THREADS 32

// To implement memory barrier (flush)
//void __atomic_thread_fence(int);
#define mb() __atomic_thread_fence(3)

// Type declaration for Internal Control Variables (ICV) structure
typedef struct {
  bool debug_enabled;
  int nthreads_var;
} miniomp_icv_t;

extern miniomp_icv_t miniomp_icv;

extern miniomp_parallel_t *miniomp_parallel;
extern pthread_mutex_t *miniomp_parallel_mutex;
extern unsigned miniomp_parallel_count;
extern unsigned miniomp_thread_count;

// Other global functions
void CHECK_ERR(int result, int expected); 

void DEBUG(char *str);

#endif
