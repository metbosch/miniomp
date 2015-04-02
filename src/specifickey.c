#include <pthread.h>
#include "specifickey.h"

// Struct definition
struct miniomp_specifickey_struct {
  unsigned id;
  miniomp_parallel_t *parallel_region;
  miniomp_specifickey_t *previous;
};

pthread_key_t miniomp_specifickey;

miniomp_specifickey_t *new_miniomp_specifickey_t(unsigned id, miniomp_parallel_t *region) {
  miniomp_specifickey_t *ret = (miniomp_specifickey_t *) malloc (sizeof (miniomp_specifickey_t));
  ret->id = id;
  ret->parallel_region = region;
  ret->previous = NULL;
  return ret;
}

void destroy_miniomp_specifickey_t(miniomp_specifickey_t *key) {
  free(key);
}

void destroy_miniomp_specifickey(void * arg) {
  destroy_miniomp_specifickey_t((miniomp_specifickey_t *)arg);
}

inline miniomp_specifickey_t *miniomp_get_thread_specifickey() {
  return (miniomp_specifickey_t *)(pthread_getspecific(miniomp_specifickey));
}

inline unsigned miniomp_get_thread_id() {
  return ((miniomp_specifickey_t *)(pthread_getspecific(miniomp_specifickey)))->id;
}

inline miniomp_parallel_t *miniomp_specifickey_get_parallel(miniomp_specifickey_t *key) {
  return key->parallel_region;
}

inline miniomp_parallel_t *miniomp_get_parallel_region() {
  return miniomp_specifickey_get_parallel((miniomp_specifickey_t *)(pthread_getspecific(miniomp_specifickey)));
}

void miniomp_set_thread_specifickey(miniomp_specifickey_t *key) {
  key->previous = (miniomp_specifickey_t *)(pthread_getspecific(miniomp_specifickey));
  pthread_setspecific(miniomp_specifickey, (void *)(key));
}

miniomp_specifickey_t *miniomp_pop_thread_specifickey() {
  miniomp_specifickey_t *key = ((miniomp_specifickey_t *)(pthread_getspecific(miniomp_specifickey)));
  pthread_setspecific(miniomp_specifickey, (void *)(key->previous));
  return key;
}

void miniomp_specifickey_create() {
  CHECK_ERR( pthread_key_create(&miniomp_specifickey, destroy_miniomp_specifickey), 0 );
}

