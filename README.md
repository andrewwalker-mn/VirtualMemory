# Running and Testing the Code

All code can be compiled with a simple **make** command. From there, you can run the provided virtmem program as specified, with  **./virtmem npages nframes rand|fifo|custom scan|sort|focus**

Outside of just manually testing the code, we provide two ways to test its functionality. 

**benchmark.py** is a python script that runs virtmem at each combination of algorithm and test program, using 100 pages, and iterating through [1, 2, 10, 25, 50, 100] frames. It takes the output of those runs and graphs them, saving them to testfocus.png, testscan.png, and testsort.png. Because running our custom algorithm on the sort program creates a very high number of page faults and disk reads, there are options to comment out certain lines to produce a more interpretable sort graph. Obviously, this can be run with python3 benchmark.py.

**newtests.sh** is a bash script that runs virtmem at each combination of algorithm and test program, iterating through [2 5 10 20 50] pages and [2 5 10] frames. It prints the output of each run, which is just the number of page faults, the number of disk reads, and the number of disk writes. It's useful for actually compparing numbers at different pages and numbers, although the output is quite lengthy. 
