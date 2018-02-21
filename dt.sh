#!/bin/bash

# Module load gcc compiler version 4.8.2
module load  gcc/4.9.2

# Runs a bunch of standard command-line
# utilities, just as an example:

echo "Script began:" `date`
echo "Current directory: ${PWD}"

#Clear files first
> inputTest.txt
for trial in 1 2 3 4 5 ; do
  ./dt input.pgm out1.pgm >> inputTest.txt
done
#cat inputTest.txt|head -10|tr " " "\t"|cut -f13|awk 'f += $1 {printf("%.13g\n",f/NR)}'|tail -1
awk ' { sum += $1; n++ } END { print sum/n }' inputTest.txt
echo "=== Done! ==="
