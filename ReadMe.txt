Team

Shyam Naren Kandala (76842367) - snkandal

Karan J Mehta (54125423) - karanjm


Quick Sort Algorithm

The quick sort algorithm is parallelized by creating new tasks for each new recursive call to quicksort function. 
The code starts with depth equal to number of maximum threads, which reduces by a factor of 2 with each call. 
When the depth reaches 1, the code shifts to a sequential sorting mechanism as it is not feasible to launch more threads than actual number of cores. 
This code has a work of O (n * log (n)) and depth of O (n) 

The current algorithm sorts around 12 million keys per second whereas sequential takes around 4 million keys per second, thus achieving a speedup of around 3. 
Thus the parallellism is O (log (n)).
The experiment was run with 8 threads. 

Merge Sort Algorithm

Results:
Sequential Sort: 4.1 million Keys / second
2 Threads: 	 8.3 million Keys / second
4 Threads:       14.7 million Keys / second
8 Threads:       18.9 million Keys / second

