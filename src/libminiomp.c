#include <unistd.h>
#include "libminiomp.h"
#include "intrinsic.h"
#include "specifickey.h"
#include "parallel.h"
#include <signal.h>
#include <execinfo.h>

// Library constructor and desctructor
void init_miniomp(void) __attribute__((constructor));
void fini_miniomp(void) __attribute__((destructor));

// Function to parse OMP_NUM_THREADS environment variable
void parse_env(void);

void CHECK_ERR(int result, int expected) {
  if (result != expected) {
    printf("ERROR DETECTED:\texpected return code: %d, returned: %d\n", expected, result);
    exit(result);
  }
}

int miniomp_debug_atomic = 0;
void DEBUG(char *str) {
  if (miniomp_icv.debug_enabled) {
    while (!(__sync_bool_compare_and_swap(&miniomp_debug_atomic, 0, 1)));
    fprintf(stderr, "%d:\t", miniomp_get_thread_id());
    fprintf(stderr, str);
    fprintf(stderr, "\n");
    miniomp_debug_atomic = 0;
  }
}

void handler(int signalCode) {
  //CHECK_ERR(-1000000, 0);
  void *array[10];
  size_t size;
  char **strings;
  size_t i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);

  printf ("SEGMENTATION FAULT: Obtained %zd stack frames.\n", size);

  for (i = 0; i < size; i++)
     printf ("%s\n", strings[i]);

  free (strings);
  exit(0);
}

void
init_miniomp(void) {
  DEBUG("Mini-omp is being initialized\n");
  parse_env();
  
  miniomp_parallel_create();
  miniomp_specifickey_create();
  miniomp_set_thread_specifickey(new_miniomp_specifickey_t(0, NULL));
//  signal(SIGSEGV, handler);
}

void
fini_miniomp(void) {
  DEBUG("Mini-omp is finalized\n");
}
