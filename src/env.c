#ifndef __MINIOMP_ENV_C__
#define __MINIOMP_ENV_C__
#include <unistd.h>
#include <string.h>
#include "libminiomp.h"

// Global variable storing the ICV (internal control variables) supported in our implementation
miniomp_icv_t miniomp_icv;

void parse_env(void) {
   char * env = getenv("OMP_NUM_THREADS");
   int procs = (int)sysconf( _SC_NPROCESSORS_ONLN ); // returns the number of cores in system
   miniomp_icv.nthreads_var = (procs <= 0 ? 1 : procs);
   if (env != NULL) {
      int env_threads = atoi(env);
      miniomp_icv.nthreads_var = (env_threads > 0 ? env_threads : miniomp_icv.nthreads_var);
   }

   char * dbg_env = getenv("OMP_DEBUG");
   miniomp_icv.debug_enabled = true;
   dbg_env = (dbg_env != NULL) ? dbg_env : "FALSE";
   miniomp_icv.debug_enabled &= (strcmp(dbg_env, "FALSE") != 0);
   miniomp_icv.debug_enabled &= (strcmp(dbg_env, "false") != 0);
   miniomp_icv.debug_enabled &= (strcmp(dbg_env, "False") != 0);
}

#endif
