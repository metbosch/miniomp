#ifndef __MINIOMP_ATOMIC_QUEUE_H__
#define __MINIOMP_ATOMIC_QUEUE_H__
#include <pthread.h>
#include "libminiomp.h"

typedef struct miniomp_queue_elem_struct miniomp_queue_elem_t;

struct miniomp_queue_elem_struct {
  void *data;
  miniomp_queue_elem_t *next;
};

typedef struct {
  pthread_mutex_t mutex;
  miniomp_queue_elem_t *front;
  miniomp_queue_elem_t *last;
} miniomp_atomic_queue_t;

miniomp_atomic_queue_t *new_miniomp_atomic_queue_t();

void destroy_miniomp_atomic_queue_t(miniomp_atomic_queue_t *queue);

void miniomp_atomic_queue_init(miniomp_atomic_queue_t *queue);

void miniomp_atomic_queue_destroy(miniomp_atomic_queue_t *queue);

void miniomp_atomic_queue_push(miniomp_atomic_queue_t *queue, void *element);

void *miniomp_atomic_queue_pop(miniomp_atomic_queue_t *queue);

bool miniomp_atomic_queue_empty(miniomp_atomic_queue_t *queue);

#endif
