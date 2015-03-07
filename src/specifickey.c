#include <pthread.h>
#include "specifickey.h"

pthread_key_t miniomp_specifickey;

miniomp_specifickey_t *new_miniomp_specifickey_t(unsigned id, miniomp_parallel_t *region) {
  miniomp_specifickey_t *ret = (miniomp_specifickey_t *) malloc (sizeof (miniomp_specifickey_t));
  ret->id = id;
  ret->parallel_region = region;
  return ret;
}

void destroy_miniomp_specifickey_t(miniomp_specifickey_t *key) {
  free(key);
}

void destroy_miniomp_specifickey(void * arg) {
  destroy_miniomp_specifickey_t((miniomp_specifickey_t *)arg);
}

miniomp_specifickey_t *miniomp_get_thread_specifickey() {
  return (miniomp_specifickey_t *)(pthread_getspecific(miniomp_specifickey));
}

void miniomp_set_thread_specifickey(miniomp_specifickey_t *key) {
  pthread_setspecific(miniomp_specifickey, (void *)(key));
}

void miniomp_specifickey_create() {
  CHECK_ERR( pthread_key_create(&miniomp_specifickey, destroy_miniomp_specifickey), 0 );
}

