#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "array.h"
#include "problem-spec.h"

void show_usage_and_exit(char *progname)
{
	fprintf(stderr, "Usage:  %s T n s\n", progname);
	fprintf(stderr, "    T : twb solve over 0<=t<=T\n");
	fprintf(stderr, "    n : number of gridpoints a = x[0],x[1],"
			"... , x[n], x[n+1] = b\n");
	fprintf(stderr, "    s : number of time-slices 0 = t[0],t[1],"
			" ... , t[n], t[s] = T\n");
	exit(EXIT_FAILURE);
}

static void plot_curve(FILE *fp, double *u, int n, int steps, int k) 
{
	for(int j=0; j<n+2; j++)
		fprintf(fp, "%g %g %g\n", (double)k/steps, (double)j/(n+1), u[j]);
}

static double get_error(struct problem_spec *spec, double *u, int n, double T)
{
	double err = 0.0;
	for (int j=0; j<n+2; j++) {
		double x = spec->a + (spec->b - spec->a)/(n+1)*j;
		double diff = fabs(u[j] - spec->u_exact(x,T));
		if (diff > err)
			err = diff;
	}

	return err;
}

static void heat_explicit(struct problem_spec *spec, double T, int n,
		int steps, char *gv_filename)
{
	FILE *fp;
	double *u, *v;
	double dx = (spec->b - spec->a)/(n+1);
	double dt =  T/steps;
	double r = dt/(dx*dx);
	double *temp;
	if ((fp = fopen(gv_filename, "w")) == NULL) {
		fprintf(stderr, "unable to open file '%s' for writing \n", gv_filename);
		return;
	}

	if(r>.5)
		printf("Warning Finite Difference method unstable");

	fprintf(fp, "# geomview script written by the function %s()\n",
			__func__);
	fprintf(fp, "{ appearance { +edge }\n");
	fprintf(fp, "MESH %d %d\n", n+2, steps+1);
	printf("%g < x < %g, 0<t<%g, dx =%g, dt =%g, "
			"r = dt/(dx*dx) = %g\n", spec->a, spec->b, 
			T, dx, dt, r);

	make_vector(u, n+2);
	make_vector(v, n+2);

	for (int j = 0; j < n+2; j++) {
		double x = spec->a + (spec->b -spec->a)/(n+1)*j;
		u[j] = spec->ic(x);
	}


	for (int i=0; i<=steps; i++){
		for (int j=1; j <=n; j++) 
			v[j] = r*u[j-1] + (1-2*r)*u[j] + r*u[j+1];
		temp = v;
		v = u;		
		u = temp;
		double t = T*i/steps;
		v[0] = spec->bcL(t);
		v[n+1] = spec->bcR(t);
		plot_curve(fp, u ,n, steps, i);
	}
		

	fprintf(fp, "}\n");
	fclose(fp);
	printf("geomview script written to file %s\n", gv_filename);
	if(spec->u_exact != NULL) {
		double err = get_error(spec, u, n, T);
		printf("max error at time %g is %g\n", T, err);
	}

	free_vector(u);
	free_vector(v);
	putchar('\n');
}

int main(int argc, char **argv)
{
	struct problem_spec *heat1(void);
	struct problem_spec *heat2(void);
	struct problem_spec *heat3(void);
	struct problem_spec *heat4(void);
	char *endptr;
	double T;
	int n, steps;
	
	if (argc != 4) {
		show_usage_and_exit(argv[0]);
	}

	T = strtod(argv[1], &endptr);
	
	if (*endptr != '\0' || T <= 0.0) {
		show_usage_and_exit(argv[0]);
	}

	n = strtol(argv[2], &endptr, 10);

	if (*endptr != '\0' || n < 1.0) {
		show_usage_and_exit(argv[0]);
	}

	steps = strtol(argv[3], &endptr, 10);

	if (*endptr != '\0' || steps < 0.0) {
		show_usage_and_exit(argv[0]);
	}

	heat_explicit(heat1(), T, n, steps, "im1.gv");
	heat_explicit(heat2(), T, n, steps, "im2.gv");
	heat_explicit(heat3(), T, n, steps, "im3.gv");
	heat_explicit(heat4(), T, n, steps, "im4.gv");
	return EXIT_SUCCESS;
}

