#include "libminiomp.h"
#include "intrinsic.h"
#include "specifickey.h"

/* This routine is called when first encountering a SINGLE construct. 
   Returns true if this is the thread that should execute the clause.  */

bool miniomp_single_first() {
  miniomp_specifickey_t *info = miniomp_get_thread_specifickey();
  unsigned prev = __sync_fetch_and_add(&info->parallel_region->single_count, 1);
  if (prev == (info->parallel_region->num_threads - 1)) {
    info->parallel_region->single_count = 0;
  }
  return !prev;
}

bool miniomp_single_last() {
  miniomp_specifickey_t *info = miniomp_get_thread_specifickey();
  unsigned prev = __sync_fetch_and_add(&info->parallel_region->single_count, 1);
  if (prev == (info->parallel_region->num_threads - 1)) {
    info->parallel_region->single_count = 0;
    return true;
  }
  return false;
}

bool
GOMP_single_start (void)
{
  return miniomp_single_first();
}


