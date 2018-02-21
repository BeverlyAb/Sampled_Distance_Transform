#!/bin/bash

# Module load gcc compiler version 4.8.2
module load  gcc/4.9.2

# Runs a bunch of standard command-line
# utilities, just as an example:
#Clear files first
> inputTest.txt
for trial in 1 2 3 4 5 ; do
  ./dt input.pgm out1.pgm >> inputTest.txt
done
#display /Users/fullstackmachine/Desktop/BevCode/224_HPC/Project/teamDT/result_images/res_vg.pgm
awk ' { sum += $1; n++ } END { print sum/n }' inputTest.txt
echo "=== Done! ==="
