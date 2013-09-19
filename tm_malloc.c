/*
 *  TRACEMALLOC
 *
 *  Author: Pradeep Subrahmanion
 *
 *  Date  :  1/1/2012
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <malloc.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include "allocation.h"

void *dlmalloc(size_t size);



int 	   tm_line_number     = 0;
const char *tm_function_name  = "";
char 	   *tm_file_name      = "";
bool       registered_exit ;

/*
 *  This method updates the line number ,file name and function name of the latest 'malloc' call
 */

void tm_update_data(int line_number,const char *function_name,char *file_name)
{
    tm_file_name =(char *) sbrk(strlen(file_name)-3); /*remove _tm from file name*/
    strncpy(tm_file_name,file_name,strlen(file_name)-5);
    strcat(tm_file_name,".c");
    tm_line_number 	 = line_number;
    tm_function_name     = function_name;
}

/*
 * TraceMalloc implementation of standard library function 'malloc'
 *
 */

void * malloc(size_t size)
{
    void *ptr = dlmalloc(size);  /*allocate memory */
    add_allocation_node(ptr,tm_line_number,size,tm_file_name,tm_function_name); /*register allocation*/
    if(!registered_exit) {
        atexit(report_allocations); /*at exit of the program , report leaked blocks*/
        registered_exit = true;
    }
    return ptr;
}

/*
 * TraceMalloc implementation of standard library function 'free'
 */

void free(void * ptr)
{
    remove_allocation_node(ptr); /* remove allocation from register*/
    dlfree(ptr);        /* deallocate memory */
}

/*
 * This method calls free from  libc
 * NOT USED NOW
 */
#if 0
void  std_free(void * ptr)
{
    char *error;
    void  (*std_free_ptr)(void *);
    dlerror();
    *(void **)(&std_free_ptr) = dlsym(RTLD_NEXT,"free");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "symbol - %s\n", error);
        exit(EXIT_FAILURE);
    }
    (*std_free_ptr)(ptr);
}
#endif
