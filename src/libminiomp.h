#ifndef __MINIOMP_LIB_H__
#define __MINIOMP_LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

#define MAX_THREADS 32

// Type declaration for Internal Control Variables (ICV) structure
typedef struct {
  bool debug_enabled;
  int nthreads_var;
} miniomp_icv_t;

extern miniomp_icv_t miniomp_icv;

// Forward of types declaration
typedef struct miniomp_parallel_struct miniomp_parallel_t;
typedef struct miniomp_thread_team_struct miniomp_thread_team_t;
typedef struct miniomp_thread_struct miniomp_thread_t;

// Other global functions
void CHECK_ERR(int result, int expected); 
void DEBUG(char *str);

#endif
