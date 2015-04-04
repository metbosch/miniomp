#include "specifickey.h"
#include "parallel.h"
#include "loop.h"
#include "single.h"

bool
GOMP_loop_dynamic_next (long *istart, long *iend)
{
  miniomp_loop_t *loop = *(miniomp_parallel_get_loop(miniomp_get_parallel_region()));
  return miniomp_loop_dynamic_next(loop, istart, iend);
}

bool
GOMP_loop_dynamic_start (long start, long end, long incr, long chunk_size,
                         long *istart, long *iend)
{
  miniomp_parallel_t *region = miniomp_get_parallel_region();
  miniomp_loop_t **loop = miniomp_parallel_get_loop(region);
  if (miniomp_single_first()) {
    *loop = new_miniomp_loop_t(start, end, incr, chunk_size, ws_DYNAMIC);
    __sync_synchronize();
  }
  miniomp_parallel_barrier_wait(region);
  return miniomp_loop_dynamic_next(*loop, istart, iend);
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
