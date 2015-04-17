#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

#define RECURSIVE 3
#define PARALLEL  4
#define LEVELS    12

long result=0;
long result2=0;

void foo(int level) {
   if (level == 0) {
      #pragma omp task shared(result)
      {
         #pragma omp atomic
         ++result;
      }
      
      #pragma omp task shared(result2)
      {
         #pragma omp critical
         ++result2;
      }
//      #pragma omp taskwait
   } else {
      for (int i = 0; i < RECURSIVE; ++i) {
         #pragma omp task shared(result, result2)
         {
            foo(level - 1);
         }
      }
   }
}

int main(int argc, char *argv[]) {
   long t1, t2, t3, t4, t5;
   t1 = t2 = t3 = t4 = t5 = 0;
   for(int i = 0; i < PARALLEL; ++i) {
      #pragma omp parallel
      {
         foo(LEVELS);
      }
   }
   printf("Result: %ld,\tResult2: %ld\n", result, result2);
}
