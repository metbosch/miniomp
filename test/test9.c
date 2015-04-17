#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

int first=0, second=0, third=0;

int foo() {
    int i, x = 1023;
#pragma omp parallel firstprivate(x) reduction(+:first)
    {
    x++; 
    first += x;
    }

#pragma omp parallel private(i) shared(first) reduction(+:second) num_threads(2)
{
    second = 14;
    #pragma omp for
    for (i = 0; i < 16; i++) {
        #pragma omp atomic
        second++;
   }
}
#pragma omp parallel reduction(+:third)
{
third++;
#pragma omp single
  #pragma omp parallel num_threads(3)
    printf("Thread %d finished the execution of foo\n", omp_get_thread_num());
}
    return(x);
}

int main(int argc, char *argv[]) {
#pragma omp parallel num_threads(2)
#pragma omp single
{
    #pragma omp task
    foo();

    #pragma omp task
    foo();

    #pragma omp taskwait
    printf("first = %d, second = %d, third = %d\n", first, second, third);
}
}
