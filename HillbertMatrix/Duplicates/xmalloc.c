#include<stdlib.h>
#include<stdio.h>
#include "xmalloc.h"

void *malloc_or_exit(size_t nbytes, char *filename, int lineno)
{
	void *x;

	if (nbytes != 0) { //if the request is nonzero execute 
		x = malloc( nbytes );
	} else { //Prints an error if it fails with file and line number
		fprintf(stderr, "%s: %d: malloc() of cannot allocate 0 bytes.\n", filename, lineno);
		exit(EXIT_FAILURE);
	}
	if (x == NULL) {//Checks to see if x is null or not
		fprintf(stderr, "%s: %d: malloc() of %zu bytes failed\n", filename, lineno, nbytes);
		exit(EXIT_FAILURE);
	} else {
		return x;
	}
}
