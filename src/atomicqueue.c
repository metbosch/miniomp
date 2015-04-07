#ifndef __MINIOMP_ATOMIC_QUEUE_C__
#define __MINIOMP_ATOMIC_QUEUE_C__
#include "atomicqueue.h"
#include "libminiomp.h"
#include <pthread.h>

// Structs definition
struct miniomp_queue_elem_struct {
   void *data;
   miniomp_queue_elem_t *next;
};

/*struct miniomp_atomic_queue_struct {
   pthread_mutex_t mutex;
   miniomp_queue_elem_t *front;
   miniomp_queue_elem_t *last;
};*/

void miniomp_atomic_queue_init(miniomp_atomic_queue_t *queue) {
   queue->front = NULL;
   queue->last = NULL;
   CHECK_ERR( pthread_mutex_init(&queue->mutex, NULL), 0 );
}

void miniomp_atomic_queue_destroy(miniomp_atomic_queue_t *queue) {
   CHECK_ERR( pthread_mutex_destroy(&queue->mutex), 0 );
   queue->last = NULL;
   while (queue->front != NULL) {
      miniomp_queue_elem_t *front = queue->front;
      queue->front = front->next;
      free(front);
   }
}

miniomp_atomic_queue_t *new_miniomp_atomic_queue_t() {
   miniomp_atomic_queue_t *ret = (miniomp_atomic_queue_t *)(malloc(sizeof(miniomp_atomic_queue_t)));
   miniomp_atomic_queue_init(ret);
   return ret;
}

void destroy_miniomp_atomic_queue_t(miniomp_atomic_queue_t *queue) {
   miniomp_atomic_queue_destroy(queue);
   free(queue);
}

miniomp_queue_elem_t *new_queue_elem_t(void *data, miniomp_queue_elem_t *next) {
   miniomp_queue_elem_t *ret = (miniomp_queue_elem_t *)(malloc(sizeof(miniomp_queue_elem_t)));
   ret->data = data;
   ret->next = next;
   return ret;
}

void miniomp_atomic_queue_push(miniomp_atomic_queue_t *queue, void *element) {
   CHECK_ERR( pthread_mutex_lock(&queue->mutex), 0 );
   if (queue->last == NULL) {
      queue->front = new_queue_elem_t(element, NULL);
      queue->last = queue->front;
   } else {
      queue->last->next = new_queue_elem_t(element, NULL);
      queue->last = queue->last->next;
   }
   CHECK_ERR( pthread_mutex_unlock(&queue->mutex), 0 );
}


void *miniomp_atomic_queue_pop(miniomp_atomic_queue_t *queue) {
   if (miniomp_atomic_queue_empty(queue)) {
      return NULL;
   }
   CHECK_ERR( pthread_mutex_lock(&queue->mutex), 0 );
   void * element = NULL;
   if (!miniomp_atomic_queue_empty(queue)) {
      miniomp_queue_elem_t *front = queue->front;
      element = front->data;
      queue->front = front->next;
      free(front);
      queue->last = miniomp_atomic_queue_empty(queue) ? NULL : queue->last;
   }
   CHECK_ERR( pthread_mutex_unlock(&queue->mutex), 0 );
   return element;
}

inline bool miniomp_atomic_queue_empty(miniomp_atomic_queue_t *queue) {
   return queue->front == NULL;
}

#endif
