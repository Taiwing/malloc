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

> This will result in an shared object file. This file will be named
> *libft_malloc_[hostype].so* where hostype is the type of machine and kernel
> of the host. A *libft_malloc.so* symbolic link to this file will be created
> at the same time. It is to be used for running programs with these malloc
> functions.

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

List of malloc functions calls and their parameters while the program is
running.

```shell
MALLOC_HISTORY=1 LD_PRELOAD=./libft_malloc.so ls -l
```

partial output on a regular *ls* call:

```
malloc(size = 20)
malloc(size = 21)
malloc(size = 21)
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

Show the state of the allocated memory at each function call.

```shell
MALLOC_SHOW=1 LD_PRELOAD=./libft_malloc.so ls -l
```

example output:

```
malloc(size = 120)
TINY : 0x7f5f0459b020
0x7f5f0459b040 - 0x7f5f0459b0c0 : 128 bytes
Total : 128 bytes

malloc(size = 12)
TINY : 0x7f5f0459b020
0x7f5f0459b040 - 0x7f5f0459b0c0 : 128 bytes
0x7f5f0459b0e0 - 0x7f5f0459b0f0 : 16 bytes
Total : 144 bytes

malloc(size = 776)
TINY : 0x7f5f0459b020
0x7f5f0459b040 - 0x7f5f0459b0c0 : 128 bytes
0x7f5f0459b0e0 - 0x7f5f0459b0f0 : 16 bytes
SMALL : 0x7f5f04231020
0x7f5f04231040 - 0x7f5f04231350 : 784 bytes
Total : 928 bytes
```
