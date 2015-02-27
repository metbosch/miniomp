#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

long result=0;

void foo() {
#pragma omp parallel shared(result) 
    {
    for (long i = 0; i < 10; i++)
        #pragma omp critical
        result++;
    #pragma omp barrier

    #pragma omp atomic
    result++;

    #pragma omp barrier
    printf("result = %ld\n", result);
    }
}

int main(int argc, char *argv[]) {
    foo();
}
