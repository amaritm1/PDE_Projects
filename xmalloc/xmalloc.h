#ifndef H_XMALLOC_H
#define H_XMALLOC_H
#include<stdlib.h>
#define xmalloc(n) malloc_or_exit(n, __FILE__, __LINE__)

void *malloc_or_exit(size_t nbytes, char *filename, int lineno);

#endif
