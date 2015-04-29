#ifndef __MINIOMP_ATOMIC_QUEUE_H__
#define __MINIOMP_ATOMIC_QUEUE_H__
#include <stdbool.h>
#include <pthread.h>

/**
  * Types definition
  */
typedef struct miniomp_queue_elem_struct miniomp_queue_elem_t;
//typedef struct miniomp_atomic_queue_struct miniomp_atomic_queue_t;
typedef struct {
   pthread_mutex_t mutex;
   miniomp_queue_elem_t *front;
   miniomp_queue_elem_t *last;
} miniomp_atomic_queue_t;

/**
  * Creates a new atomic queue object
  */
miniomp_atomic_queue_t *new_miniomp_atomic_queue_t();

/**
  * Destroys an existing object
  */
void destroy_miniomp_atomic_queue_t(miniomp_atomic_queue_t *queue);

/**
  * Initializes an existing atomic queue object
  */
void miniomp_atomic_queue_init(miniomp_atomic_queue_t *queue);

/**
  * Frees and uninitializes an existing atomic queue object
  */
void miniomp_atomic_queue_destroy(miniomp_atomic_queue_t *queue);

/**
  * Pushes a new element into the atomic queue
  */
void miniomp_atomic_queue_push(miniomp_atomic_queue_t *queue, void *element);

/**
  * Getter of top queue element
  * Returns NULL if the queue is emprty
  */
void *miniomp_atomic_queue_pop(miniomp_atomic_queue_t *queue);

/**
  * Tests if the queue is empty
  */
bool miniomp_atomic_queue_empty(miniomp_atomic_queue_t *queue);

#endif
