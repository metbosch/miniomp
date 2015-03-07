#ifndef __MINIOMP_LIB_H__
#define __MINIOMP_LIB_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "barrier.h"
#include "workdescriptor.h"
#include "loop.h"

#define MAX_THREADS 32

// To implement memory barrier (flush)
//void __atomic_thread_fence(int);
#define mb() __atomic_thread_fence(3)

// Type declaration for Internal Control Variables (ICV) structure
typedef struct {
  int nthreads_var;
  // Add other ICV if needed
} miniomp_icv_t;

extern miniomp_icv_t miniomp_icv;

// Type declaration for parallel descriptor (arguments needed to create pthreads)
//typedef struct miniomp_parallel_struct miniomp_parallel_t;
typedef struct {
    int id;
    // complete the definition of parallel descriptor
    miniomp_wd_t wd;
    pthread_t *threads;		// Threads associated to this parallel region
    unsigned num_threads;	// Number of threads executing the parallel region
    miniomp_barrier_t barrier;  // Barrier for this parallel region
    pthread_mutex_t mutex;      // Mutex for critical regions
    unsigned single_count;      // Counter for single regions
    miniomp_loop_t *loop;	// Pointer to parallel loop structure
    pthread_mutex_t loop_mutex;	// Mutex for loop creation and destruction
} miniomp_parallel_t;

extern miniomp_parallel_t *miniomp_parallel;
extern pthread_mutex_t *miniomp_parallel_mutex;
extern unsigned miniomp_parallel_count;
extern unsigned miniomp_thread_count;

typedef struct {
    int max_elements;
    int count;
    int head;
    int tail;
    int first;
    int lock_queue;
    miniomp_wd_t **queue;
    // complete with additional field if needed
} miniomp_taskqueue_t;

extern miniomp_taskqueue_t * miniomp_taskqueue;
#define MAXELEMENTS_TQ 128

// funtions to implement basic management operations on taskqueue
bool is_empty(miniomp_taskqueue_t *task_queue);
bool is_full(miniomp_taskqueue_t *task_queue) ;
bool is_valid(miniomp_wd_t *task_descriptor);
bool enqueue(miniomp_taskqueue_t *task_queue, miniomp_wd_t *task_descriptor); 
bool dequeue(miniomp_taskqueue_t *task_queue);
miniomp_wd_t *first(miniomp_taskqueue_t *task_queue);

// Other global functions
void CHECK_ERR(int result, int expected); 

#endif
