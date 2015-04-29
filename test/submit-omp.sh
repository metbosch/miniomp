#!/bin/bash
### Directives pel gestor de cues
# following option makes sure the job will run in the current directory
#$ -cwd
# following option makes sure the job has the same environmnent variables as the submission shell
#$ -V
# Canviar el nom del job
##$ -N lab1-omp
# Per canviar de shell
#$ -S /bin/bash

USAGE="\n USAGE: ./submit-omp prog range slice_size num_threads\n
        prog        -> omp program name\n
        range       -> count primes between 2 and range\n
        slice_size  -> size of the blocks to sieve\n
        num_threads -> number of threads\n"

if (test $# -lt 4 || test $# -gt 4)
then
	echo -e $USAGE
        exit 0
fi

make $1-omp

/usr/bin/time ./$1-omp $2 $3 $4
