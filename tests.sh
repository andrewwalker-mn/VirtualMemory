#!/bin/bash

counter=4
while [ $counter -le 10 ]
do
echo ./virtmem $counter 2 rand sort
./virtmem $counter 2 rand sort
echo ./virtmem $counter 2 custom sort
./virtmem $counter 2 custom sort
((counter++))
done

counter=4
while [ $counter -le 10 ]
do
echo ./virtmem $counter 4 rand sort
./virtmem $counter 4 rand sort
echo ./virtmem $counter 4 custom sort
./virtmem $counter 4 custom sort
((counter++))
done
echo ______________________________________________________________________________________________________
counter=4
while [ $counter -le 30 ]
do
echo ./virtmem $counter 2 rand focus
./virtmem $counter 2 rand focus
echo ./virtmem $counter 2 custom focus
./virtmem $counter 2 custom focus
((counter+=5))
done


counter=4
while [ $counter -le 30 ]
do
echo ./virtmem $counter 4 rand focus
./virtmem $counter 4 rand focus
echo ./virtmem $counter 4 custom focus
./virtmem $counter 4 custom focus
((counter+=5))
done
echo ______________________________________________________________________________________________________
counter=4
while [ $counter -le 30 ]
do
echo ./virtmem $counter 2 rand scan
./virtmem $counter 2 rand scan
echo ./virtmem $counter 2 custom scan
./virtmem $counter 2 custom scan
((counter+=5))
done

counter=4
while [ $counter -le 30 ]
do
echo ./virtmem $counter 4 rand scan
./virtmem $counter 4 rand scan
echo ./virtmem $counter 4 custom scan
./virtmem $counter 4 custom scan
((counter+=5))
done

echo fuck you
