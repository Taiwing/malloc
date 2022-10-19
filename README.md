# malloc

Implementation of the *malloc* function in C. This function dynamically
allocates memory for C programs. The *free* function is also provided as well as
*realloc* which changes the size of previous memory allocation, and
*show_alloc_mem* which displays information about the state of the allocated
memory zones.

<br />
<p align="center">
	<img src="https://github.com/Taiwing/malloc/blob/master/resources/malloc.webp?raw=true" alt="Kardue'sai'Malloc" style="width: 50%;"/>
</p>

## Setup

```shell
# clone this project
git clone https://github.com/Taiwing/malloc
# build it
cd malloc/ && make
```

This will result in an shared object file. This file will be named
*libft_malloc_[hostype].so* where hostype is the type of machine and kernel of
the host. A *libft_malloc.so* symbolic link to this file will be created at the
same time. It is to be used for running programs with these malloc functions.

### Thread safety

The malloc implementation provided by this library is NOT thread safe by
default. To activate this feature you just have to set the *THREAD_SAFE*
environment variable at compile time.

```shell
# rerun make with THREAD_SAFE on
THREAD_SAFE=1 make re
```

## Usage

### Run a program with this malloc implementation

Set the *LD_PRELOAD* environment variable to make a program use this library
instead of the system's default.

```shell
# run a random program
LD_PRELOAD=./libft_malloc.so ls -l
```

Which behaves as expected:

```
total 28
drwxr-xr-x 2 yforeau 2018_paris 4096 Oct 19 13:00 includes
drwxr-xr-x 7 yforeau 2018_paris 4096 Oct 19 13:31 libft
-rw-r--r-- 1 yforeau 2018_paris 1671 Sep 23 22:56 Makefile
-rw-r--r-- 1 yforeau 2018_paris 1177 Oct 19 13:31 README.md
drwxr-xr-x 2 yforeau 2018_paris 4096 Sep 20 19:37 resources
drwxr-xr-x 2 yforeau 2018_paris 4096 Oct 19 13:09 src
-rw-r--r-- 1 yforeau 2018_paris  214 Sep 20 19:37 TODO
```

### Malloc's debug environment variables

This implementation of malloc provides a list of environment variables to be set
at run time.

#### MALLOC_HISTORY

List of malloc functions calls, their parameters and their eventual return value
while the program is running.

```shell
MALLOC_HISTORY=1 LD_PRELOAD=./libft_malloc.so ls -l
```

partial output on a regular *ls* call:

```
malloc(size = 20) --> 0x7fcb326259e0
malloc(size = 21) --> 0x7fe18fd41c80
malloc(size = 21) --> 0x7fe18fd42660
free(ptr = 0x0)
free(ptr = 0x7fe18fd41c80)
free(ptr = 0x0)
free(ptr = 0x7fe18fd42660)
drwxr-xr-x 2 yforeau 2018_paris   4096 Oct 19 13:00 includes
drwxr-xr-x 8 yforeau 2018_paris   4096 Oct 19 13:40 libft
lrwxrwxrwx 1 yforeau 2018_paris     28 Oct 19 13:40 libft_malloc.so -> libft_malloc_x86_64_Linux.so
-rwxr-xr-x 1 yforeau 2018_paris 118240 Oct 19 13:40 libft_malloc_x86_64_Linux.so
-rw-r--r-- 1 yforeau 2018_paris   1671 Sep 23 22:56 Makefile
drwxr-xr-x 2 yforeau 2018_paris   4096 Oct 19 13:40 obj
-rw-r--r-- 1 yforeau 2018_paris   1889 Oct 19 13:42 README.md
drwxr-xr-x 2 yforeau 2018_paris   4096 Sep 20 19:37 resources
drwxr-xr-x 2 yforeau 2018_paris   4096 Oct 19 13:42 src
-rw-r--r-- 1 yforeau 2018_paris    214 Sep 20 19:37 TODO
free(ptr = 0x7fe18f4a4040)
```

#### MALLOC_SHOW

Show a list of allocation blocks per zone at each function call (check the
[How it works](#how-it-works) section for an in-depth explanation of the
output).

```shell
MALLOC_SHOW=1 LD_PRELOAD=./libft_malloc.so ls -l
```

example output:

```
malloc(size = 120) --> 0x7f5f0459b040
TINY : 0x7f5f0459b020
0x7f5f0459b040 - 0x7f5f0459b0c0 : 128 bytes
Total : 128 bytes

malloc(size = 12) --> 0x7f5f0459b0e0
TINY : 0x7f5f0459b020
0x7f5f0459b040 - 0x7f5f0459b0c0 : 128 bytes
0x7f5f0459b0e0 - 0x7f5f0459b0f0 : 16 bytes
Total : 144 bytes

malloc(size = 776) --> 0x7f5f04231040
TINY : 0x7f5f0459b020
0x7f5f0459b040 - 0x7f5f0459b0c0 : 128 bytes
0x7f5f0459b0e0 - 0x7f5f0459b0f0 : 16 bytes
SMALL : 0x7f5f04231020
0x7f5f04231040 - 0x7f5f04231350 : 784 bytes
Total : 928 bytes
```

The address following the title of each section is the start of the zone (TINY,
SMALL and LARGE are the types of the zones). Then each line is an allocation.
The left address is the start of the allocated memory block (the address
actually returned by the malloc functions), and the right one is where the block
ends. The size in bytes is showed at the end of the line.

## How it works

This implementation relies on the *mmap()*, *mremap()* and *munmap()* functions
to dynamically allocate, reallocate and free memory. Allocations are separated
in three types depenging on their size. By default every allocation smaller than
512 bytes are TINY, smaller than 4096 bytes (and bigger than 512) are SMALL and
everything above is LARGE.

For performance reasons this implementation will limit at a minimum the number
of calls to the underlying memory functions. The first time that *malloc()* is
called *mmap()* will be used to allocate a memory zone able to store at least
128 allocations by default. Then a block will be allocated in the zone to store
the allocation. For the following allocations *mmap()* will not have to be
called and a block will directly be allocated if available. LARGE allocations
work differently. For this type of memory request a new zone will have to be
created for each allocation because there is no upper bound on their size. It is
guaranteed that each allocated block and resulting address will be sixteen
aligned.

*mmunap()* is called when deleting an empty zone if an other one of the same
type still exists. However LARGE zones are directly deleted when *free()* is
called.
