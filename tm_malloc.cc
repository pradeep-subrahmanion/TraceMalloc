#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include "allocation_register.h"

// This method calls the  malloc from standard library
// This method calls malloc from  libc.so.6

bool insideLibrary;
 
void * std_malloc(size_t size)
{



    char *error;

    void * (*std_malloc_ptr)(size_t);

    dlerror();
 
   *(void **)(&std_malloc_ptr) = dlsym(RTLD_NEXT,"malloc");

    if ((error = dlerror()) != NULL)  {
        fprintf(stderr, "symbol - %s\n", error);
       exit(EXIT_FAILURE);
    }

    void *p =  (*std_malloc_ptr)(size);

   return p;
}


// TraceMalloc implementation of standard library function 'malloc'

void * malloc(size_t size)
{

 void *ptr=NULL;

  if(!insideLibrary)
{

    static int i;

    //printf("starting\n");

    insideLibrary = true;   
    
    ptr = std_malloc(size);
 
    //printf("after std malloc\n");

   // Register allocation

    allocation_register::register_new_allocation((char *)"test.cpp",size,ptr,100);
  
   //printf("after reg\n");

   allocation_register::report_allocations();

   //printf("after report\n");

   insideLibrary = false;

   ++i;
  }

  else
 {
     ptr = std_malloc(size);
     //printf("after inside\n");
 }

   return ptr;

}

