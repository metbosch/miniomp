#include <unistd.h>
#include "libminiomp.h"
#include <string.h>

// Global variable storing the ICV (internal control variables) supported in our implementation
miniomp_icv_t miniomp_icv;

void parse_env(void) {
    char * env = getenv("OMP_NUM_THREADS");
    //printf("Environment variable OMP_NUM_THREADS not defined, setting nthreads_var ICV to number of cores\n");
    int procs = (int)sysconf( _SC_NPROCESSORS_ONLN ); // returns the number of cores in system
    miniomp_icv.nthreads_var = (procs <= 0 ? 1 : procs);
    if (env != NULL) {
      int env_threads = atoi(env);
      miniomp_icv.nthreads_var = (env_threads > 0 ? env_threads : miniomp_icv.nthreads_var);
    }
    //printf("Setting nthreads_var ICV to %d\n", miniomp_icv.nthreads_var);
    char * dbg_env = getenv("OMP_DEBUG");
    miniomp_icv.debug_enabled = (dbg_env != NULL);
    miniomp_icv.debug_enabled &= (strcmp(dbg_env, "FALSE") != 0);
    miniomp_icv.debug_enabled &= (strcmp(dbg_env, "false") != 0);
    miniomp_icv.debug_enabled &= (strcmp(dbg_env, "False") != 0);
}
