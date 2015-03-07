#include "libminiomp.h"
#include "specifickey.h"

void omp_set_num_threads (int n) {
  miniomp_icv.nthreads_var = (n > 0 ? n : 1);
}

int omp_get_num_threads (void) {
  miniomp_specifickey_t * key = miniomp_get_thread_specifickey();
  return(key->parallel_region == NULL ? 1 : key->parallel_region->num_threads);
}

int omp_get_thread_num (void) {
  //if (miniomp_specifickey == NULL) return 1;
  return (miniomp_get_thread_specifickey()->id);
  //printf("TBI: omp_get_thread_num still doesn't know who I am ... let's say 0\n");
  //return(0);
}
