#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"

int main(void) {
	void* cat = xmalloc(1000); 
	//Allocates 1000 bytes in dummy variable
	
	fprintf(stdout,"A 1000 bytes of memory have been allocated.");
	
	//tells user that memory has been allocated
	
	free(cat);
	
	//Frees memeory back up 
	
	fprintf(stdout, "The memory of your cat was freed.");
	
	void* penguin = xmalloc(0);//Allocates 0 bytes of memeory
	
	fprintf(stdout," 0 bytes of memory have been allocated.");
	
	free(penguin); //Frees the zero memory 
	
	fprintf(stdout, "Your penguin was freed."); //Frees the zero memory
	
	return 0;
}

