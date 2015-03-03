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

//Forward declaration of parallel region type
typedef struct miniomp_parallel_struct miniomp_parallel_t;

// Type declaration of per-thread specific key
typedef struct {
  unsigned id;
  miniomp_parallel_t *parallel_region;
} miniomp_specifickey_t;

typedef miniomp_specifickey_t * miniomp_specifickey_ptr;

// declaration of per-thread specific key
extern pthread_key_t miniomp_specifickey;

// Type declaration for parallel descriptor (arguments needed to create pthreads)
//typedef struct miniomp_parallel_struct miniomp_parallel_t;
struct miniomp_parallel_struct {
    int id;
    // complete the definition of parallel descriptor
    miniomp_wd_t wd;
    pthread_t *threads;		// Threads associated to this parallel region
    unsigned num_threads;
    miniomp_barrier_t barrier;  // Barrier for this parallel region
    pthread_mutex_t mutex;      // Mutex fot critical regions
};

extern miniomp_parallel_t *miniomp_parallel;
extern pthread_mutex_t *miniomp_parallel_mutex;
extern unsigned miniomp_parallel_count;
extern unsigned miniomp_thread_count;


// Type declaration for loop worksharing descriptor
typedef struct {
  long start;           // loop bounds and increment 
  long end;
  long incr;

  int schedule;         // schedule kind for loop
  long chunk_size;
  // complete the definition of worksharing descriptor
} miniomp_loop_t;

#define ws_STATIC 	0
#define ws_STATICCHUNK 	1
#define ws_DYNAMIC 	2
#define ws_GUIDED 	3
#define ws_RUNTIME 	4
#define ws_AUTO 	5

extern miniomp_loop_t miniomp_loop;

// Type declaration for single work sharing descriptor
typedef struct {
    // complete the definition of single descriptor
} miniomp_single_t;

// Declaratiuon of global variable for single work descriptor
extern miniomp_single_t miniomp_single;

/* This structure describes a "task" to be run by a thread.  */
typedef struct {
    void (*fn)(void *);
    void (*data);
    // complete with additional field if needed
} miniomp_task_t;

typedef struct {
    int max_elements;
    int count;
    int head;
    int tail;
    int first;
    int lock_queue;
    miniomp_task_t **queue;
    // complete with additional field if needed
} miniomp_taskqueue_t;

extern miniomp_taskqueue_t * miniomp_taskqueue;
#define MAXELEMENTS_TQ 128

// funtions to implement basic management operations on taskqueue
bool is_empty(miniomp_taskqueue_t *task_queue);
bool is_full(miniomp_taskqueue_t *task_queue) ;
bool is_valid(miniomp_task_t *task_descriptor);
bool enqueue(miniomp_taskqueue_t *task_queue, miniomp_task_t *task_descriptor); 
bool dequeue(miniomp_taskqueue_t *task_queue);
miniomp_task_t *first(miniomp_taskqueue_t *task_queue);

// Other global functions
void CHECK_ERR(int result, int expected); 

#endif
