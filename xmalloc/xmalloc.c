#include<stdlib.h>
#include<stdio.h>
#include "xmalloc.h"

void *malloc_or_exit(size_t nbytes, char *filename, int lineno)
{
	void *x;

	//if the request is nonzero execute 
	if (nbytes != 0) { 
		x = malloc(nbytes);
	}
	//Prints an error if it fails with file and line number
	else { 
		fprintf(stderr, "\n%s: %d: malloc() of cannot allocate 0 bytes.\n", filename, lineno);
		exit(EXIT_FAILURE);
	}
	//Checks to see if x is null or not
	if (x == NULL) {
		fprintf(stderr, "\n%s: %d: malloc() of %zu bytes failed\n", filename, lineno, nbytes);
		exit(EXIT_FAILURE);
	} 
	else {
		return x;
	}
}
