#include "libminiomp.h"
#include "intrinsic.h"
#include "specifickey.h"
#include "parallel.h"

/* This routine is called when first encountering a SINGLE construct. 
   Returns true if this is the thread that should execute the clause.  */

bool miniomp_single_first() {
  miniomp_specifickey_t *info = miniomp_get_thread_specifickey();
  unsigned prev = __sync_fetch_and_add(miniomp_parallel_get_single_count(info->parallel_region), 1);
  if (prev == (miniomp_parallel_get_num_threads(info->parallel_region) - 1)) {
    *(miniomp_parallel_get_single_count(info->parallel_region)) = 0;
  }
  return !prev;
}

bool miniomp_single_last() {
  miniomp_specifickey_t *info = miniomp_get_thread_specifickey();
  unsigned prev = __sync_fetch_and_add(miniomp_parallel_get_single_count(info->parallel_region), 1);
  if (prev == (miniomp_parallel_get_num_threads(info->parallel_region) - 1)) {
    *(miniomp_parallel_get_single_count(info->parallel_region)) = 0;
    return true;
  }
  return false;
}

bool
GOMP_single_start (void)
{
  return miniomp_single_first();
}


