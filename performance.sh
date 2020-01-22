#!/bin/bash

if [ -d performance ]; then	rm -r performance;	fi
mkdir performance
type=0
for file in selection.txt insertion.txt merge.txt quick.txt hybrid.txt
do
	for i in {1000,5000,10000,50000,75000,100000,500000}
	do
		./output $type in/${i}.txt 0.txt time.txt
		while read -r line;	do 	echo "${i}_unsorted: $line ms" >> performance/$file;	done < time.txt
		./output $type 0.txt 1.txt time.txt
		while read -r line;	do 	echo "${i}_sorted: $line ms" >> performance/$file; echo >> performance/$file; done < time.txt
	done
	type=$(($type+1))
done

rm 0.txt
rm 1.txt	
rm time.txt

