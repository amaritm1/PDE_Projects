#include <stdio.h>
#include "nelder-mead.h"

static double obj_func(double *x, int n, void *params)
{
	return x[0]*x[0] + x[1]*x[1] - 4*x[0] - 2*x[1] + 8;
}

int main (void)
{
	double x[]= { 0.0, 0.0 }; //initial point
	int evalcount;
	struct nelder_mead NM = { // need C99 for the initialization
		.f = obj_func,    // object function
		.n = 2,           // the dimension of the space
		.s = NULL,        // constructs s
		.x = x,           // initial or final point
		.h = 0.1,         // problems scale
		.tol = 1.0e-4,    // tolerance
		.maxevals = 1000, // maximum number of evaluations
		.params = NULL,   // no parameters
	};

	evalcount = nelder_mead(&NM);
	if(evalcount > NM.maxevals) {
		printf("No convergence after %d function evaluations\n",
				evalcount);
	} else {
		printf("Converged after %d function evaluations\n",evalcount);
		printf("Computed solution: min = %g at (%g,%g)\n",
				NM.minval, x[0], x[1]);
	}
	return 0;
}
