#ifndef __MINIOMP_INTRINSIC_C__
#define __MINIOMP_INTRINSIC_C__
#include "libminiomp.h"
#include "specifickey.h"
#include "parallel.h"

void omp_set_num_threads (int n) {
   miniomp_icv.nthreads_var = (n > 0 ? n : 1);
}

int omp_get_num_threads (void) {
   miniomp_parallel_t * p = miniomp_get_parallel_region();
   return (p == NULL ? 1 : miniomp_parallel_get_num_threads(p));
}

int omp_get_thread_num (void) {
   return miniomp_get_thread_id();
}

#endif
