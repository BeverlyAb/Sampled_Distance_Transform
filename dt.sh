#!/bin/bash
#$ -q class8i
# -pe mpi 8 
#$ -R y
#$ -N DTS
#$ -pe openmp 8			
# Module load gcc compiler version 4.8.2
#module load  gcc/4.9.2

# Runs a bunch of standard command-line
# utilities, just as an example:
#Clear files first
> inputTest.txt
for trial in 1 2 3 4 5 ; do
  ./dt input.pgm out1.pgm >> inputTest.txt
	#mpirun -np 8 ./dt input.pgm out1.pgm >> inputTest.txt
done
#display -resize 25% result_images/res_vg.pgm &
awk ' { sum += $1; n++ } END { print sum/n }' inputTest.txt
echo "=== Done! ==="
