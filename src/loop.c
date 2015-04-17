#ifndef __MINIOMP_LOOP_C__
#define __MINIOMP_LOOP_C__
#include "libminiomp.h"
#include "intrinsic.h"
#include "loop.h"
#include "single.h"
#include "specifickey.h"
#include "parallel.h"

#define MIN(x, y) ((y) ^ (((x) ^ (y)) & -((x) < (y))))

miniomp_loop_t * new_miniomp_loop_t(long start, long end, long incr, long chunk_size, int schedule) {
   miniomp_loop_t *ret = (miniomp_loop_t *)(malloc(sizeof(miniomp_loop_t)));
   ret->start = start;
   ret->end = end;
   ret->incr = incr;
   ret->schedule = schedule;
   ret->chunk_size = ((incr >= 0) == (chunk_size >= 0)) ? chunk_size : -chunk_size;
   ret->next = start;
   return ret;
}

void destroy_miniomp_loop_t(miniomp_loop_t * loop) {
   if ((loop->next < loop->end && loop->incr >= 0) || (loop->next > loop->end && loop->incr < 0)) {
      printf("ERROR: Trying to destroy a loop structure with work (next: %lu, end: %lu)\n", loop->next, loop->end);
   }
   free(loop);
}

bool miniomp_loop_dynamic_next(long *istart, long *iend) {
   miniomp_loop_t *loop = *(miniomp_parallel_get_loop(miniomp_get_parallel_region()));
   long increment = loop->chunk_size;
   *istart = __sync_fetch_and_add(&loop->next, increment);
   *iend = *istart + increment;
   if (loop->incr >= 0) {
      *iend = MIN(*iend, (loop->end));
      return(*istart < (loop->end));
   } else {
      *iend = MAX(*iend, (loop->end));
      return(*istart > (loop->end));
   }
}

bool
GOMP_loop_dynamic_next (long *istart, long *iend) {
   return miniomp_loop_dynamic_next(istart, iend);
}

bool
GOMP_loop_dynamic_start (long start, long end, long incr, long chunk_size,
long *istart, long *iend)
{
   miniomp_parallel_t *region = miniomp_get_parallel_region();
   if (miniomp_single_first()) {
      *(miniomp_parallel_get_loop(region)) = new_miniomp_loop_t(start, end, incr, chunk_size, ws_DYNAMIC);
      __sync_synchronize();
   }
   miniomp_parallel_barrier_wait(region);
   return miniomp_loop_dynamic_next(istart, iend);
}

void
GOMP_loop_end (void)
{
   miniomp_parallel_t *region = miniomp_get_parallel_region();
   miniomp_loop_t **loop = miniomp_parallel_get_loop(region);
   if (miniomp_parallel_barrier_wait(region)) {
      destroy_miniomp_loop_t(*loop);
      *loop = NULL;
      __sync_synchronize();
   }
   miniomp_parallel_barrier_wait(region);
}

void
GOMP_loop_end_nowait (void)
{
   if (miniomp_single_last()) {
      miniomp_parallel_t *region = miniomp_get_parallel_region();
      miniomp_loop_t **loop = miniomp_parallel_get_loop(region);
      destroy_miniomp_loop_t(*loop);
      *loop = NULL;
      __sync_synchronize();
   }
}

#endif
