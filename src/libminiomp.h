#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>

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

// declaration of per-thread specific key
extern pthread_key_t miniomp_specifickey;

// Default lock for critical sections
extern pthread_mutex_t miniomp_default_lock;

// Default barrier within a parallel region
#define MYBARRIER 0
#if MYBARRIER
//  Here you should define all neceesary types and variables to implement your barrier
#else
// Default barrier using Pthread barriers 
extern pthread_barrier_t miniomp_barrier;
#endif

// Type declaration for own implementation of barrier
typedef struct {
    // complete the definition of barrier
} miniomp_barrier_t;

//extern miniomp_barrier_t miniomp_barrier;

// Declaration of array for storing pthread identifier from pthread_create function
extern pthread_t *miniomp_threads;

// Type declaration for parallel descriptor (arguments needed to create pthreads)
typedef struct {
    void (*fn) (void *);
    void *fn_data;
    int id;
    // complete the definition of parallel descriptor
    pthread_t *threads;		// Threads associated to this parallel region
} miniomp_parallel_t;

extern miniomp_parallel_t *miniomp_parallel;

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
