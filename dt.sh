#!/bin/bash
#$ -q class8i
#$ -R y
#$ -N DTS
#$ -pe openmp 8			
# Module load gcc compiler version 4.8.2
#module load  gcc/4.9.2

####### Chunk Test	####### 
for ((j=0; j<3; j+=1)); do
	for	((i=31; i<33; i+=10)); do
		./dt /data/users/babadine/HPC_224/project/teamDT_v2/img/newAll/ /data/users/babadine/HPC_224/project/teamDT_v2/3_20/sizeOut/ $i >> /data/users/babadine/HPC_224/project/teamDT_v2/3_20/512/$i.txt
	done
done

####### Row test	####### 
#./dt /data/users/babadine/HPC_224/project/teamDT_v2/img/newAll/ /data/users/babadine/HPC_224/project/teamDT_v2/3_20/sizeOut/ 100 >> /data/users/babadine/HPC_224/project/teamDT_v2/3_20/chunkData/pad.txt


####### Function test	####### 
#./dt /data/users/babadine/HPC_224/project/teamDT_v2/img/newAll/ /data/users/babadine/HPC_224/project/teamDT_v2/3_20/sizeOut/ 100 >> inputTest.txt
#awk ' { sum += $1; n++ } END { print sum/n }' inputTest.txt
#echo "=== Done! ==="

