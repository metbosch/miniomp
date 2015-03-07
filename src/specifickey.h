#ifndef __MINIOMP_SPECIFICKEY_H__
#define __MINIOMP_SPECIFICKEY_H__
#include "libminiomp.h"

// Type declaration of per-thread specific key
typedef struct {
  unsigned id;
  miniomp_parallel_t *parallel_region;
} miniomp_specifickey_t;

miniomp_specifickey_t *new_miniomp_specifickey_t(unsigned id, miniomp_parallel_t *region);

void destroy_miniomp_specifickey_t(miniomp_specifickey_t *key);

miniomp_specifickey_t *miniomp_get_thread_specifickey();

void miniomp_set_thread_specifickey(miniomp_specifickey_t *key);

void miniomp_specifickey_create();

#endif
