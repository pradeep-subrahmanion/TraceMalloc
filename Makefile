libtracemalloc: allocation_list.o tm_malloc.o dlmalloc.o tracemalloc
	ar -rvs libtracemalloc.a dlmalloc.o tm_malloc.o allocation_list.o 

tracemalloc:tracemalloc.c
	gcc tracemalloc.c -o tracemalloc

dlmalloc.o:dlmalloc.c
	gcc -c dlmalloc.c

tm_malloc.o: tm_malloc.c
	gcc -c tm_malloc.c

allocation_list.o: allocation.h allocation_list.c
	gcc -c allocation_list.c


