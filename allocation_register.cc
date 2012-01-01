#include "allocation_register.h"  
#include <iostream>

std::list<allocation> allocation_register::allocation_list;

void allocation_register::register_new_allocation(char *file_name,size_t size,void * address,int line_number)
{
  

  allocation *alc = new allocation();

  alc->file_name = file_name;

  alc->address = address;

  alc->line_number = line_number;

  alc->size = size;


  allocation_register::allocation_list.push_back(*alc);

}

void allocation_register::deregister_allocation(unsigned int address)
{

}

void allocation_register::report_allocations()
{

   FILE *fd = fopen("log.txt","w");
    
   fprintf(fd,"%s","File\t\tLine Number\tSize\tAddress\n---------------------------------------\n");

   std::list<allocation>::iterator it;

   it = allocation_register::allocation_list.begin();

   while(it!=allocation_register::allocation_list.end())
   {
        allocation atn= *it;

        fprintf(fd,"%s\t\t%d\t%ld\t%u\n",atn.file_name,atn.line_number,atn.size,atn.address);

       ++it;      

   }

  fclose(fd);

}
