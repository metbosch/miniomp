#include "libminiomp.h"
#include "intrinsic.h"
#include "loop.h"
#include "single.h"
#include "specifickey.h"

#define MIN(x, y) ((y) ^ (((x) ^ (y)) & -((x) < (y))))

miniomp_loop_t * new_miniomp_loop_t(long start, long end, long incr, long chunk_size, int schedule) {
  miniomp_loop_t *ret = (miniomp_loop_t *)(malloc(sizeof(miniomp_loop_t)));
  ret->start = start;
  ret->end = end;
  ret->incr = incr;
  ret->schedule = schedule;
  ret->chunk_size = chunk_size;
  ret->next = start;
  ret->end_count = 0;
  return ret;
}

void destroy_miniomp_loop_t(miniomp_loop_t * loop) {
  if (loop->next < loop->end) {
    printf("ERROR: Trying to destroy a loop structure with work (next: %lu, end: %lu)\n", loop->next, loop->end);
  }
  free(loop);
}

bool miniomp_loop_dynamic_next(long *istart, long *iend) {
  miniomp_loop_t *loop = *(miniomp_parallel_get_loop(miniomp_get_thread_specifickey()->parallel_region)); 
  long increment = (loop->incr * loop->chunk_size);
  *istart = __sync_fetch_and_add(&loop->next, increment);
  *iend = *istart + increment;
  *iend = MIN(*iend, (loop->end));
  return(*istart < (loop->end));
}

/* The *_next routines are called when the thread completes processing of 
   the iteration block currently assigned to it.  If the work-share 
   construct is bound directly to a parallel construct, then the iteration
   bounds may have been set up before the parallel.  In which case, this
   may be the first iteration for the thread.

   Returns true if there is work remaining to be performed; *ISTART and
   *IEND are filled with a new iteration block.  Returns false if all work
   has been assigned.  */

bool
GOMP_loop_dynamic_next (long *istart, long *iend) {
  return miniomp_loop_dynamic_next(istart, iend);
}

/* The *_start routines are called when first encountering a loop construct
   that is not bound directly to a parallel construct.  The first thread 
   that arrives will create the work-share construct; subsequent threads
   will see the construct exists and allocate work from it.

   NSTART, END, INCR are the bounds of the loop; CHUNK_SIZE is the
   scheduling parameter. 

   Returns true if there's any work for this thread to perform.  If so,
   *ISTART and *IEND are filled with the bounds of the iteration block
   allocated to this thread.  Returns false if all work was assigned to
   other threads prior to this thread's arrival.  */

bool
GOMP_loop_dynamic_start (long start, long end, long incr, long chunk_size,
                         long *istart, long *iend)
{
  miniomp_parallel_t *region = miniomp_get_thread_specifickey()->parallel_region;
  if (miniomp_single_first()) {
    
    *(miniomp_parallel_get_loop(region)) = new_miniomp_loop_t(start, end, incr, chunk_size, ws_DYNAMIC);
    __sync_synchronize();
  }
  miniomp_parallel_barrier_wait(region);
  return miniomp_loop_dynamic_next(istart, iend);
}

/* The GOMP_loop_end* routines are called after the thread is told that
   all loop iterations are complete.  The first version synchronize
   all threads; the nowait version does not. */

void
GOMP_loop_end (void) {
  miniomp_parallel_t *region = miniomp_get_thread_specifickey()->parallel_region; 
  miniomp_loop_t **loop = miniomp_parallel_get_loop(region);
  if (miniomp_parallel_barrier_wait(region)) {
    destroy_miniomp_loop_t(*loop);
    *loop = NULL;
    __sync_synchronize();
  }
  miniomp_parallel_barrier_wait(region);
}

void
GOMP_loop_end_nowait (void) {
  if (miniomp_single_last()) {
    miniomp_parallel_t *region = miniomp_get_thread_specifickey()->parallel_region;
    miniomp_loop_t **loop = miniomp_parallel_get_loop(region);
    destroy_miniomp_loop_t(*loop);
    *loop = NULL;
    __sync_synchronize();
  }
}
