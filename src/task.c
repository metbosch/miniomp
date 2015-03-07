#include "libminiomp.h"

miniomp_taskqueue_t * miniomp_taskqueue;

// Initializes the task queue
miniomp_taskqueue_t *init_task_queue(int max_elements) {
    return NULL;
}

// Checks if the task descriptor is valid
bool is_valid(miniomp_wd_t *task_descriptor) {
    return false;
}

// Checks if the task queue is empty
bool is_empty(miniomp_taskqueue_t *task_queue) {
    return true;
}

// Checks if the task queue is full
bool is_full(miniomp_taskqueue_t *task_queue) {
    return false;
}

// Enqueues the task descriptor at the tail of the task queue
bool enqueue(miniomp_taskqueue_t *task_queue, miniomp_wd_t *task_descriptor) {
    return true;
}

// Dequeue the task descriptor at the head of the task queue
bool dequeue(miniomp_taskqueue_t *task_queue) { 
    return true;
}

// Returns the task descriptor at the head of the task queue
miniomp_wd_t *first(miniomp_taskqueue_t *task_queue) {
    return NULL;
}

/* Called when encountering an explicit task directive.  If IF_CLAUSE is
   false, then we must not delay in executing the task. */
void
GOMP_task (void (*fn) (void *), void *data, void (*cpyfn) (void *, void *),
           long arg_size, long arg_align, bool if_clause, unsigned flags,
           void **depend)
{
    printf("TBI: a task has been encountered, I am executing it immediately\n");
    if (__builtin_expect (cpyfn != NULL, 0))
        {
	  char * buf =  malloc(sizeof(char) * (arg_size + arg_align - 1));
          char *arg = (char *) (((uintptr_t) buf + arg_align - 1)
                                & ~(uintptr_t) (arg_align - 1));
          cpyfn (arg, data);
          fn (arg);
        }
    else
        fn (data);
}

void
GOMP_taskwait (void)
{
    printf("TBI: Entered in taskwait, there should be no pending tasks, so I proceed\n");
}
