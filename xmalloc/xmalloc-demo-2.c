#include<stdlib.h>
#include<stdio.h>
#include"xmalloc.h"
int main(){
	while (1) {
		//Sets up infinite loop
		
		xmalloc(1000000000);
		//allocates one gigabyte of memory
		
		fprintf(stdout, "allocated one gigabyte memory.\n");
	}	//tells user the memory has been allocated
return 0;
}
