#ifndef __MINIOMP_WD_H__
#define __MINIOMP_WD_H__

/**
  * Structures and types definition
  */
typedef struct {
   void (*fn) (void *);
   void *fn_data;
} miniomp_wd_t;

/**
  * Creates a new work descriptor object
  */
miniomp_wd_t *new_miniomp_wd_t(void (*fn) (void *), void *fn_data);

/**
  * Destroys an existing object
  */
void destroy_miniomp_wd_t(miniomp_wd_t *wd);

/**
  * Initializes an existing work descriptor object
  */
void miniomp_wd_init(miniomp_wd_t *wd, void (*fn) (void *), void *fn_data);

/**
  * Executes the work descriptor function
  */
void miniomp_wd_run(miniomp_wd_t * wd);

#endif
