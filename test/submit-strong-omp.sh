#!/bin/bash
### Directives pel gestor de cues
# following option makes sure the job will run in the current directory
#$ -cwd
# following option makes sure the job has the same environmnent variables as the submission shell
#$ -V
# Canviar el nom del job
#$ -N miniomp-strong
# Per canviar de shell
#$ -S /bin/bash

USAGE="\n USAGE: ./submit-strong.sh prog\n
          prog -> openmp program name\n"

if (test $# -lt 1 || test $# -gt 1)
then
	echo -e $USAGE
        exit 0
fi

PROG=$1
np_NMIN=7
np_NMAX=12
N=3

# Make sure that all binaries exist
make $PROG

out=/tmp/out.$$	    # Temporal file where you save the execution results
aux=/tmp/aux.$$     # archivo auxiliar

outputpath=./elapsed-$PROG.txt
rm -rf $outputpath 2> /dev/null

echo "$PROG $size $np_NMIN $np_NMAX $N"
export MINIOMP="/scratch/nas/1/pap1103/sessions/miniomp"

i=0
echo "Starting OpenMP executions..."

PARS=$np_NMIN
while (test $PARS -le $np_NMAX)
do
	echo Executing $PROG with $PARS threads 
	elapsed=0  # Acumulacion del elapsed time de las N ejecuciones del programa
        export OMP_NUM_THREADS=$PARS

	while (test $i -lt $N)
		do
			echo -n Run $i... 
			/usr/bin/time --format=%e ./$PROG > $out 2>$aux

			time=`cat $aux|tail -n 1`
			echo Elapsed time = `cat $aux`
			
			elapsed=`echo $elapsed + $time|bc`
			
			rm -f $out
			rm -f $aux
			i=`expr $i + 1`
		done

	echo -n ELAPSED TIME AVERAGE OF $N EXECUTIONS =

    	average=`echo $elapsed/$N|bc -l`
    	echo $average
	echo
	i=0

    	#output PARS i average en fichero elapsed time
	#echo -n $PARS >> $outputpath
	#echo -n "   " >> $outputpath
    	echo $average >> $outputpath

    	#incrementa el parametre
	PARS=`expr $PARS + 1`
done

echo "Resultat de l'experiment (tambe es troben a " $outputpath " )"
echo "#threads	Elapsed average"
cat $outputpath
echo

