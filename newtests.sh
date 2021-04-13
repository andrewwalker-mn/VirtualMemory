#!/bin/bash
for page in 2 5 10 20 50
do
  for frame in 2 5 10
    do
    echo ./virtmem $page $frame rand sort
    ./virtmem $page $frame rand sort
    echo ./virtmem $page $frame custom sort
    ./virtmem $page $frame custom sort
    done
done

echo _____________________________________________________________________________________________________

#!/bin/bash
for page in 2 5 10 20 50
do
  for frame in 2 5 10
    do
    echo ./virtmem $page $frame rand scan
    ./virtmem $page $frame rand scan
    echo ./virtmem $page $frame custom scan
    ./virtmem $page $frame custom scan
    done
done

echo _____________________________________________________________________________________________________

#!/bin/bash
for page in 2 5 10 20 50
do
  for frame in 2 5 10
    do
    echo ./virtmem $page $frame rand focus
    ./virtmem $page $frame rand focus
    echo ./virtmem $page $frame custom focus
    ./virtmem $page $frame custom focus
    done
done
