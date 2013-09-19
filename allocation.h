/*
 *  TRACEMALLOC
 * 
 *  Author: Pradeep Subrahmanion
 * 
 *  Date  :  1/1/2012
 */

#include <stdio.h>

/*
 * This structure holds information about each allocation.
 */

struct allocation 
{
    void  * address;
    char  *file_name;
    const char  *function_name ;
    int   line_number;
    size_t   size;
};

 void add_allocation_node(void * address,int line_no,size_t size,char *file_name,const char *function_name);
 void remove_allocation_node(void * address);
 void report_allocations();
 void tm_update_data(int line_number,const char *function_name,char *file_name);

