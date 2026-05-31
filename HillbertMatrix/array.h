#ifndef H_ARRAY_H
#define H_ARRAY_H
#include<stdlib.h>
#include<stdio.h>
#include "xmalloc.h" //includes stdio.h and stdlib.h

//Defining Macro for making a vector. Allocates memeory or appropriate size
//Inputs 1: anytype* v; 
// 	 2: int n; length of vector
#define make_vector(v,n) ((v) = xmalloc((n) * sizeof *(v)))
//
//Frees space of vector defined above
//
#define free_vector(v) do { free(v); v = NULL; } while (0)
//Makes matrix of dim m by n 
//Inputs:1 anytype** a; where the a is where the vecors will be stored
//	 2 int m; number of rows
//	 3 int n; number of coloumns 
#define make_matrix(a,m,n) do {\
	size_t make_matrix_loop_counter;\
	make_vector(a,(m) + 1);\
	for ( make_matrix_loop_counter = 0;\
		make_matrix_loop_counter < (m);\
		make_matrix_loop_counter++){\
		make_vector((a)[make_matrix_loop_counter], (n));}\
	(a)[m] = NULL;\
} while (0)
//
//Frees the matrix one row at a time
//
#define free_matrix(a) do {\
	if (a != NULL) {\
		size_t make_matrix_loop_counter;\
		for (make_matrix_loop_counter = 0;\
			(a)[make_matrix_loop_counter] != NULL;\
			make_matrix_loop_counter++)\
		free_vector(a[make_matrix_loop_counter]);\
		free_vector(a);\
		a=NULL;\
	}\
} while (0)
//
//Prints vector line by line
//
#define print_vector(fmt, v, n) do {\
	size_t print_vector_loop_counter;\
	for (print_vector_loop_counter = 0;\
		print_vector_loop_counter < (n);\
		print_vector_loop_counter++)\
	printf(fmt, (v)[print_vector_loop_counter]);\
	putchar('\n');\
} while (0)

// Prints matrix by calling print_vector n times

#define print_matrix(fmt, a, m, n) do{\
	size_t print_matrix_loop_counter;\
	for (print_matrix_loop_counter = 0; \
		print_matrix_loop_counter < (m);\
		print_matrix_loop_counter++){\
		print_vector(fmt, (a)[print_matrix_loop_counter], (n));\
	}\
} while (0)

//	 3 int n; number of coloumns 
#define make_order3tensor(a,m,n,k) do {\
	size_t make_3tensor_loop_counter;\
	make_matrix(a,(n) + 1,(k) + 1);\
	for ( make_3tensor_loop_counter = 0;\
		make_3tensor_loop_counter < (m);\
		make_3tensor_loop_counter++){\
		make_matrix((a)[make_3tensor_loop_counter],(n), (k));}\
	(a)[m] = NULL;\
} while (0)

//
#define free_3tensor(a) do {\
	if (a != NULL) {\
		size_t make_3tensor_loop_counter;\
		for (make_3tensor_loop_counter = 0;\
			(a)[make_3tensor_loop_counter] != NULL;\
			make_3tensor_loop_counter++)\
		free_matrix(a[make_3tensor_loop_counter]);\
		free_matrix(a);\
		a=NULL;\
	}\
} while (0)

#define print_3tensor(fmt, a, m, n, k) do{\
	size_t print_3tensor_loop_counter;\
	for (print_3tensor_loop_counter = 0; \
		print_3tensor_loop_counter < (m);\
		print_3tensor_loop_counter++)\
	print_matrix(fmt, (a)[print_3tensor_loop_counter],(n), (k));\
} while (0)

#define print_lat_slices(fmt, a, m, n, k) do{\
	size_t print_3tensor_loop_counter;\
	for (print_3tensor_loop_counter = 0; \
		print_3tensor_loop_counter < (n);\
		print_3tensor_loop_counter++)\
	print_matrix(fmt, (a)[print_3tensor_loop_counter],(m),(k));\
} while (0)

#define print_horiz_slices(fmt, a, m, n, k) do{\
	size_t print_3tensor_loop_counter;\
	for (print_3tensor_loop_counter = 0; \
		print_3tensor_loop_counter < (k);\
		print_3tensor_loop_counter++)\
	print_matrix(fmt,(a)[print_3tensor_loop_counter],(m),(n));\
} while (0)

/*
#define print_col_fibers(fmt, a, m, n, k) do{\
	size_t print_3tensor_loop_counter;\
	size_t print_3tensor_loop_counter1;\
	for (print_3tensor_loop_counter = 0; \
		print_3tensor_loop_counter < (m);\
		print_3tensor_loop_counter++){\
		for (print_3tensor_loop_counter = 0; \
			print_3tensor_loop_counter1 < (n);\
			print_3tensor_loop_counter1++)\
		print_vector(fmt,(a)[print_3tensor_loop_counter][print_3tensor_loop_counter],(k));\
	}\
} while (0)
*/


//	 3 int n; number of coloumns 
#define make_order4tensor(a,m,n,k,l) do {\
	size_t make_4tensor_loop_counter;\
	make_order3tensor(a,(n) + 1,(k)+1,(l) +1);\
	for ( make_4tensor_loop_counter = 0;\
		make_4tensor_loop_counter < (m);\
		make_4tensor_loop_counter++){\
		make_order3tensor((a)[make_4tensor_loop_counter],(n), (k), (l));}\
	(a)[m] = NULL;\
} while (0)

//
#define free_4tensor(a) do {\
	if (a != NULL) {\
		size_t make_4tensor_loop_counter;\
		for (make_4tensor_loop_counter = 0;\
			(a)[make_4tensor_loop_counter] != NULL;\
			make_4tensor_loop_counter++)\
		free_3tensor(a[make_4tensor_loop_counter]);\
		free_3tensor(a);\
		a=NULL;\
	}\
} while (0)

#define print_4tensor(fmt, a, m, n, k, l) do{\
	size_t print_4tensor_loop_counter;\
	for (print_4tensor_loop_counter = 0; \
		print_4tensor_loop_counter < (l);\
		print_4tensor_loop_counter++)\
		print_3tensor(fmt,(a)[print_4tensor_loop_counter],(m), (n), (k));\
	(a)[m] = NULL;\
} while (0)

#endif /*H_ARRAY_H*/
