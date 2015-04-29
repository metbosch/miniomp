#ifndef __MINIOMP_SINGLE_C__
#define __MINIOMP_SINGLE_C__
#include "libminiomp.h"
#include "intrinsic.h"
#include "specifickey.h"
#include "parallel.h"

bool miniomp_single_first() {
   miniomp_parallel_t *region = miniomp_get_parallel_region();
   unsigned prev = __sync_fetch_and_add(miniomp_parallel_get_single_count(region), 1);
   if (prev == (miniomp_parallel_get_num_threads(region) - 1)) {
      *(miniomp_parallel_get_single_count(region)) = 0;
   }
   return !prev;
}

bool miniomp_single_last() {
   miniomp_parallel_t *region = miniomp_get_parallel_region();
   unsigned prev = __sync_fetch_and_add(miniomp_parallel_get_single_count(region), 1);
   if (prev == (miniomp_parallel_get_num_threads(region) - 1)) {
      *(miniomp_parallel_get_single_count(region)) = 0;
      return true;
   }
   return false;
}

bool
GOMP_single_start (void)
{
   return miniomp_single_first();
}

#endif
