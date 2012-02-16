#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*
 * Create new file with trace malloc additions for showing line number , function name 
 * and file name.
 * Returns new file name
 *
 */
char * preprocess_file(char *file_name)
{
	long file_size = 0;	
	char str[200];
    int size = strlen(file_name) + sizeof(char) *3;
	char *dest_file_name = calloc(size,1);
	strncpy(dest_file_name,file_name,strlen(file_name)-2);
	strcat(dest_file_name ,"_tm.c" );
	FILE *source_fd = fopen(file_name,"r");
	FILE *dest_fd = fopen(dest_file_name,"w");
	fprintf(dest_fd,"%s\n","#include \"allocation.h\"");
	fseek(source_fd, 0, SEEK_END); /*find the size of source file */
	file_size = ftell(source_fd);
	fseek(source_fd,0,SEEK_SET);
	int prev_seek_pos =  fseek(dest_fd, 0, SEEK_END);/* read source file line by line */	
	int offset=0;
	
	while(fgets(str,file_size,source_fd)) { /*call tm_update_data before each malloc*/
		
		if(strstr(str,"malloc(") != NULL) { 
			offset++;
			int i =( offset * 4);	
	        fprintf(dest_fd,"%s","/**\ntrace malloc addition for generating debug informations\n*/\n");
			fprintf(dest_fd,"tm_update_data(__LINE__-%d,__FUNCTION__,__FILE__);\n",i);
		}
		fprintf(dest_fd,"%s",str);	
	}
		
	fclose(source_fd);
	fclose(dest_fd);
	return dest_file_name;
}


int main(int argc,char *argv[])
{
	
	char build_command[1024];
	if(argc==1) {
		printf("Usage tracemalloc <your_build_command>\n");
		printf("Eg : tracemalloc gcc test.c -o test\n");
		return 0;
	}
	
	int i=1;
	/* find .c files from command line arguments .*/
	while(i<argc) {  
		char *arg = argv[i];	
		int len = strlen(arg);
		char *new_arg = arg;               
		if(strlen(arg)>2 && arg[len-1]=='c' && arg[len-2]=='.'){
			/*
			 * preprocess .c file to generate new file which includes additional code to
			 * show line number , function name and file name of leaked block.
			 */

			new_arg = preprocess_file(arg);
		}
		
		if(i==1){
			strcpy(build_command,new_arg);		
		}
		else{
			strcat(build_command," ");
			strcat(build_command,new_arg);
		}			
		++i;
	}
	
	 strcat(build_command," -L $PWD -ltracemalloc -ldl"); /* link with trace malloc static lib */
   	 system(build_command); /* run the build command */ 
	//free(build_command);
    
	return 0;
}
