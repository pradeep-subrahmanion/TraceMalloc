/*
 *  TRACEMALLOC
 * 
 *  Author: Pradeep Subrahmanion
 * 
 *  Date  :  1/1/2012
 */

#include <unistd.h>
#include "allocation.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

///////////////////////////////////////////////////////////////////////////////////////////
// some utility functions
//

/*
 *   function to get string from integer without using any inbuilt function 
 */

char *tochar(int i, char *p)
{
    if (i / 10 == 0) {
        // No more digits.
        *p++ = i + '0';
        *p = '\0';
        return p;
    }

    p = tochar(i / 10, p);
    *p++ = i % 10 + '0';
    *p = '\0';
    return p;
}

/**
 *   function to get length of a string  without using any inbuilt function 
 */

static int wrapper_strlen (const char *str)
{
  const char *p;
  for (p = str; *p; p++);
  return p - str;
}

//////////////////////////////////////////////////////////////////////////////////////////////
/*
 *  A singly linked list is used to store allocations.
 */

struct allocation_node
{
  struct allocation_node *link;
  struct allocation *allocation_data;
};
struct allocation_node *first_node = NULL;

/*
 * Method to add an allocation at the beginning of linked list
 */
void add_allocation_node(void * address,int line_no,size_t size,char *file_name,const char *function_name)
{
  struct allocation_node *temp =  (struct allocation_node *) sbrk(sizeof(struct allocation_node));
  if(first_node != NULL) {
    temp->link = first_node;
  }
  else {
    temp->link = NULL;
  }
  
  temp->allocation_data = (struct allocation *) sbrk(sizeof(struct allocation));
  temp->allocation_data->address = address;
  temp->allocation_data->line_number = line_no;
  temp->allocation_data->size = size;
  temp->allocation_data->file_name = file_name;
  temp->allocation_data->function_name = function_name;
  first_node = temp;
}

/*
 *  Remove an allocation from list 
 */

void remove_allocation_node(void * address)
{
  struct allocation_node *it = first_node; 
  struct allocation_node *prev_node = NULL;
  while (it != NULL) {
        void * _address = it->allocation_data->address;
        if(_address == address) {
        	
        	if(prev_node == NULL) {
        	  
        		first_node = it->link;
        	}
        	else {         
        		prev_node->link = it->link;
        	}        	
          break;
        }
        prev_node = it;
        it = it->link; 
  }
}

/*
 * This method writes allocation to given file descriptor
 * 
 */

void write_to_file(int fd, struct allocation_node *it)
{
	char *mem = sbrk(8);
    char *size = sbrk(8);           
    tochar(it->allocation_data->line_number,mem);
    tochar(it->allocation_data->size,size);	     
    write(fd,it->allocation_data->file_name,wrapper_strlen(it->allocation_data->file_name));
    write(fd,"\t",1);
    write(fd,it->allocation_data->function_name,wrapper_strlen(it->allocation_data->function_name));
    write(fd,"\t\t",2);             
    write(fd,mem,wrapper_strlen(mem));
    write(fd,"\t",1);
    write(fd,size,wrapper_strlen(size));
    write(fd,"\n",1);
}

/*
 *  This method reports leaked blocks.This method generates 'log.txt' file 
 *  
 */

void report_allocations()
{
   int fd = open("log.txt",O_CREAT |O_RDWR );
   struct allocation_node *it = first_node;
   struct allocation_node *prev_node = NULL;
   write(fd,"FILE\t\tFUNCTION\tLINENO\tSIZE\n--------------------------------------------\n",72);
   write(1,"FILE\t\tFUNCTION\tLINENO\tSIZE\n--------------------------------------------\n",72);
   while (it != NULL) {	   
        write_to_file(fd,it); /* write to log.txt */
	    write_to_file(1,it);  /* write to stdout */
        it=it->link;
     }
   close(fd);
}

