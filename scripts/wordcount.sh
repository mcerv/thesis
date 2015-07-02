#!/bin/bash
# Word Counting Statistics
cd /Users/mcerv/Documents/MarieCurieTALENT/Thesis/

count=$(find . -name "*.tex" | xargs wc -w | grep 'total' | awk '{print $1'})
# time=$(date +%Y-%m-%d)
# echo -e "$time"
time=$(date +%s)
echo -e "$time \t $count"
echo -e "$time \t $count" >> wrdcnt.txt
