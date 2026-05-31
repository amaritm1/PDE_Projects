#include <suitesparse/umfpack.h>
#include <stdio.h>
#include "array.h"

void exit_on_error(int status, const char *file, int line)
{
	if (status == UMFPACK_ERROR_out_of_memory)
		fprintf(stderr, "*** file %s, line %d: "
				"UMFPACK out of memory\n", file, line);
	else if (status == UMFPACK_WARNING_singular_matrix)
		fprintf(stderr, "*** file %s, line, %d: "
				"UMFPACK recieved singular matrix\n", file, line);
	else
		fprintf(stderr, "*** file %s, line %d: "
				"trouble in UMFPACK call\n", file, line);
}

int main(void)
{

	int n = 5; // size of the matrix
	int N = n*n; // area of matrix
	double *Tx; // The value of the matrix at a given coordinate
	int *Ti, *Tj;// the row and col coordinates respectively
	double *Ax;
	int *Ap, *Ai;
	void *Numeric;
	void *Symbolic;
	double *x;
	int status; // 0 if everything works else there is a number that corresponds to an error
	make_vector(Tx, N);
	make_vector(Ti, N);
	make_vector(Tj, N);
	double B[] = { 1.0, 2.0, 3.0, 4.0, 5.0 };

	int i = 0;  //iterator goes through the area of the matrix
	for (int k = 0; i < N; k++){
		for (int j = 0; j < n; j++){
			Tj[i] = k; // saves the col value
			Ti[i] = j; // saves the row value
			Tx[i] = 1.0 / (1.0 + j + k); // computes the hilbert matrix component wise
			i++; //increments till the area of the matrix
		
		//	error checking
		//	printf("Tj[%i] = %i  \n", k, Tj[i]);
		//	printf("Ti[%i] = %i  \n", j, Ti[i]);
		//	printf("Tx[%i] = %g  \n", i, Tx[i]);

		}
	}

	make_vector(Ax, N);
	make_vector(Ai, N);
	make_vector(Ap, n+1);

	status = umfpack_di_triplet_to_col(n, n, N, Ti, Tj, Tx, Ap, Ai, Ax, NULL);
	if (status != UMFPACK_OK)
		exit_on_error(status, __FILE__, __LINE__);

	umfpack_di_symbolic(n, n, Ap, Ai, Ax, &Symbolic, NULL, NULL);
	umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, NULL, NULL);

	make_vector(x, n);

	umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, x, B, Numeric, NULL, NULL);
	printf(" Given a five by five Hilbert Matrix Hx = B with \n ");
	printf("B =");
	print_vector("%g ", B, n);

	printf(" x = ");
	print_vector("%g ", x, n);

	//Freeing vectors
	free_vector(Tx);
	free_vector(Ti);
	free_vector(Tj);
	free_vector(Ax);
	free_vector(Ai);
	free_vector(Ap);
	free_vector(x);

	umfpack_di_free_symbolic(&Symbolic);
	umfpack_di_free_numeric(&Numeric);

	return 0;
}
