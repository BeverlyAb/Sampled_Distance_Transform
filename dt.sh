#!/bin/bash
#$ -q class8i
#$ -R y
#$ -N DTS
#$ -pe openmp 8			
# Module load gcc compiler version 4.8.2
#module load  gcc/4.9.2

# Runs a bunch of standard command-line
# utilities, just as an example:
#Clear files first

echo "=== Chunksize = 100 ==="
#for trial in 1 2 3 4 5 ; do
./dt /data/users/babadine/HPC_224/project/teamDT_v2/img/newAll/ /data/users/babadine/HPC_224/project/teamDT_v2/3_20/sizeOut/ >> inputTest.txt

#done

awk ' { sum += $1; n++ } END { print sum/n }' inputTest.txt
echo "=== Done! ==="

