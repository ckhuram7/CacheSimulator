This was the third project was for my Computer Organization 
and Architecture Course. The course was taken in Fall 2013.

I was given the option to write this project using standard
C++ libraries or with the Qt libraries. I chose to use Qt.

The object of this assignment was to make a Cache Simulator
that simulated cache of different configurations.

I was given the following assumptions about all the cache
I simulated: LRU replacement strategy, cache are initially
empty, caches us allocate on write policy, and cache use 
write back policy.

We simulated cache with different settings in terms of 
cache size, block size, and mapping type. 

cache size: 1k, 4k, 64k, 128k
block size: 8, 16, 32, 128
mapping type: direct, 2-way assoc, 4-way assoc, fully assoc

To run the code simply go into the directory where
the projet is saved and on the command line type:

./simulate *name of testfile without extension*

The code should output to a file titled the same
thing as the input with an extension of .result

As far as things that work and dont work, everything
should work except the column for cache to memory. I
was having issues with the dirty bit part of the code.
