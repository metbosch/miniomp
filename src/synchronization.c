#ifndef __MINIOMP_SYNCRONIZATION_C__
#define __MINIOMP_SYNCRONIZATION_C__
#include "libminiomp.h"
#include "intrinsic.h"
#include <pthread.h>
#include "specifickey.h"
#include "parallel.h"

void
GOMP_critical_start (void)
{
   miniomp_parallel_mutex_lock(miniomp_get_parallel_region());
}

void
GOMP_critical_end ()
{
   miniomp_parallel_mutex_unlock(miniomp_get_parallel_region());
}

void
GOMP_barrier()
{
   miniomp_parallel_barrier_wait(miniomp_get_parallel_region());
}

#endif
