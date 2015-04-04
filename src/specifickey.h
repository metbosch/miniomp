#ifndef __MINIOMP_SPECIFICKEY_H__
#define __MINIOMP_SPECIFICKEY_H__
#include "libminiomp.h"

/**
  * Types definition
  */
typedef struct miniomp_specifickey_struct miniomp_specifickey_t;

/**
  * Creates a new object with the specified information
  */
miniomp_specifickey_t *new_miniomp_specifickey_t(unsigned id, miniomp_parallel_t *region);

/**
  * Destroys an existing object
  */
void destroy_miniomp_specifickey_t(miniomp_specifickey_t *key);

/**
  * Getter of specifickey for caller thread
  */
miniomp_specifickey_t *miniomp_get_thread_specifickey();

/**
  * Getter of caller thread identifier
  */
unsigned miniomp_get_thread_id();

/**
  * Getter of parallel region for caller thread
  */
miniomp_parallel_t *miniomp_get_parallel_region();
miniomp_parallel_t *miniomp_specifickey_get_parallel(miniomp_specifickey_t *key);

/**
  * Setter of specifickey for caller thread
  */
void miniomp_set_thread_specifickey(miniomp_specifickey_t *key);

/**
  * Restores the previous thread specifickey (used for nested parallel regions)
  * Returns specifickey used before the funcion
  */
miniomp_specifickey_t *miniomp_pop_thread_specifickey();

/**
  * Creates the required structures to work
  */
void miniomp_specifickey_create();

#endif
