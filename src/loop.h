#ifndef __MINIOMP_LOOP_H__
#define __MINIOMP_LOOP_H__
#include <pthread.h>

typedef struct {
  long start;           // loop bounds and increment 
  long end;
  long incr;

  int schedule;         // schedule kind for loop
  long chunk_size;
  // complete the definition of worksharing descriptor
  long next;
  int end_count;
} miniomp_loop_t;

#define ws_STATIC       0
#define ws_STATICCHUNK  1
#define ws_DYNAMIC      2
#define ws_GUIDED       3
#define ws_RUNTIME      4
#define ws_AUTO         5

miniomp_loop_t * new_miniomp_loop_t(long start, long end, long incr, long chunk_size, int schedule);

void destroy_miniomp_loop_t(miniomp_loop_t * loop);

#endif
