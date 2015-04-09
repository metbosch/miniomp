#ifndef __MINIOMP_WD_H__
#define __MINIOMP_WD_H__
#include <stdbool.h>

/**
  * Structures and types definition
  */
typedef enum{PARALLEL, TASK} miniomp_wd_kind_t;

typedef struct miniomp_wd_struct miniomp_wd_t;
struct miniomp_wd_struct {
   void (*fn) (void *);     // Function to execute
   void *fn_data;           // Data for function to execute
   miniomp_wd_t *parent;    // Parent to notify on finish
   int components;          // Number of direct childerens created
   int executed_components; // Number of direct childerens executed
   miniomp_wd_kind_t type;  // Indicates if the WD represents a task or parallel region
};


/**
  * Creates a new work descriptor object
  */
miniomp_wd_t *new_miniomp_wd_t(void (*fn) (void *), void *fn_data, miniomp_wd_t *parent, miniomp_wd_kind_t type);

/**
  * Destroys an existing object
  */
void destroy_miniomp_wd_t(miniomp_wd_t *wd);

/**
  * Initializes an existing work descriptor object
  */
void miniomp_wd_init(miniomp_wd_t *wd, void (*fn) (void *), void *fn_data, miniomp_wd_t *parent, miniomp_wd_kind_t type);

/**
  * Notifys a new descendent creation to workdescriptor
  */
void miniomp_wd_new_component(miniomp_wd_t *wd);

/**
  * Returns true if the workdescriptor has unfinished descendents
  */
bool miniomp_wd_has_components(miniomp_wd_t *wd);

/**
  * Executes the work descriptor function and destroys itself if it doesn't have descendents
  */
void miniomp_wd_run(miniomp_wd_t * wd);

#endif
