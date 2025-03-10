#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

int first=0, second=0;

int foo() {
    int i, x = 1023;
#pragma omp parallel firstprivate(x) reduction(+:first)
    {
    x++; 
    first += 1;
    }

#pragma omp parallel private(i) shared(first) reduction(+:second) num_threads(2)
{
    second = first;
    for (i = 0; i < 16; i++)
        second++;
}
#pragma omp parallel
{
#pragma omp single
  #pragma omp parallel num_threads(3)
    printf("Thread %d finished the execution of foo\n", omp_get_thread_num());
}
    return(x);
}

int main(int argc, char *argv[]) {
    printf("first = %d, second = %d, x = %d\n", first, second, foo());
}
