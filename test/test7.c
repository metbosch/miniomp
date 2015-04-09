#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

long result=0;

void foo() {
   for (long i = 0; i < 1000; i++) {
       #pragma omp task shared(result)
       #pragma omp critical
       result++;
   }
   for (long i = 0; i < 500; i++) {
       #pragma omp atomic
       result++;
   }
}

int main(int argc, char *argv[]) {
   long t1, t2, t3, t4, t5;
   t1 = t2 = t3 = t4 = t5 = 0;
   for(int i = 0; i < 1; ++i) {
      #pragma omp parallel num_threads(8)
      {
         foo();
         #pragma omp atomic
         t1 += result;
         #pragma omp barrier
         #pragma omp atomic
         t2 += result;
         #pragma omp taskwait
         #pragma omp atomic
         t3 += result;
         #pragma omp barrier
         #pragma omp atomic
         t4 += result;
      }
      t5 += result*8;
   }
   printf("Before barrier: %ld,\tBefore taskwait: %ld,\tAfter taskwait: %ld,\tAfter barrier: %ld\n", t1, t2, t3, t4);
   printf("After parallel: %ld,\tAt the end: %ld\n", t5, result);
}
