#include <stdio.h>
#include <list>
#include "allocation.h"

class allocation_register
{

  public:
 
        
	static std::list<allocation> allocation_list;

	static void register_new_allocation(char *file_name,size_t size,void * address,int line_number);

	static void deregister_allocation(unsigned int address);

        static void report_allocations();

};


