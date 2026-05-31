#include<stdio.h>
#include"xmalloc.h"
#include"array.h"

double **hilbert_matrix(int n)
{
	double** H;
	make_matrix(H, n, n);
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			H[i][j] = 1.0 / (1 + i + j);
		}
	}
	return H;
}

double ***t_tensor_test(int l, int m, int n)
{
	double*** T;
	make_order3tensor(T,l,m,n);
		for (int i = 0; i < l; i++){
			for (int j = 0; j < m; j++){
				for (int k = 0; k < n; k++){
					T[i][j][k] = i+j+k;
				}
			}
		}
	return T;
}

double ****f_tensor_test(int n)
{
	double**** F;
	make_order4tensor(F,n,n,n,n);
		for (int i = 0; i < n; i++){
			for (int j = 0; j < n; j++){
				for (int k = 0; k < n; k++){
					for (int l = 0; l < n; l++){
						F[i][j][k][l] = i;
					}
				}
			}
		}
	return F;
}

int main(void){

	double** X;
	int n;
	n = 8;
	X = hilbert_matrix(n);
	printf("This is the hilbert matrix : \n");
	print_matrix("%1.3f ", X, n, n);	
//	for (int q = 0; q < n; q++){
//	print_vector("%1.3f ", X[q], n);	
//	}
	free_matrix(X);	

	double*** T;
	int p = 2;
	int q = 3;
	int r = 4;
	T = t_tensor_test(p,q,r);
	printf("This is the 3 tensor : \n");
	print_3tensor("%1.0f",T,p,q,r);
	printf("/////////////////////////////////\n");
	print_lat_slices("%1.0f",T,p,q,r);
	printf("This is the lat slices : \n");
	printf("/////////////////////////////////\n");
	print_horiz_slices("%1.0f",T,p,q,r);
	printf("This is the horizontal slices: \n");
	printf("/////////////////////////////////\n");
//	print_col_fibers("%1.0f",T,p,q,r);
//	printf("This is the column fibers : \n");
//	printf("/////////////////////////////////\n");

	free_3tensor(T);

	double**** F;
	int x;
	x = 2;
	F = f_tensor_test(x);
	printf("This is the 4 tensor : \n");
	print_4tensor("%1.0f", F, x, x, x, x);
	free_4tensor(F);

	return 0 ;
}
	/* TEST CODE 	
	
	int* x;
	make_vector(x,n);
	for (int i = 0; i < n; i++){
		x[i] = i;
}
	printf("The vector x is : \n");
	for (int j = 0; j < n; j++){
	printf("%i\n",x[j]);}
	print_vector("%i ",x,n);	
	free_vector(x);
//	printf("%d ",*X[1]);
//	print_matrix("%f ", X, n, n);
*/


