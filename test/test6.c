#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <omp.h>	/* OpenMP */

long result=0;

void foo() {
#pragma omp task shared(result) 
    {
    for (long i = 0; i < 10; i++) {
        #pragma omp critical
        result++;
    }
}
    #pragma omp taskwait
    #pragma omp barrier
    long midle = result;
    #pragma omp barrier

    #pragma omp atomic
    result++;

    #pragma omp barrier
    printf("result = %ld, %ld\n", midle, result);

}

int main(int argc, char *argv[]) {
    #pragma omp parallel num_threads(35)
    foo();
}
