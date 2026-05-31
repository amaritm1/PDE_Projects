#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "array.h"
#include "problem-spec.h"

void show_usage_and_exit(char *progname) 
{
	fprintf(stderr, "Usage:  %s T n s\n", progname);
	fprintf(stderr, "    T : twb solve over 0<=t<=T\n");
	fprintf(stderr, "    n : number of gridpoints"
			"a = x[0],x[1], ... , x[n],x[n+1] = b\n");
	fprintf(stderr, "    s : number of time-slices 0"
			"= t[0],t[1], ... , t[n],t[s] = T\n");
	exit(EXIT_FAILURE);
}

static void trisolve(int n, double *a, double *d, double *c, double *b, double *x)
{
	for (int i=1; i<n; i++) {
		double m = a[i-1]/d[i-1];
		d[i] -= m*c[i-1];
		b[i] -= m*b[i-1];
	}

	x[n-1] = b[n-1]/d[n-1];
	for (int i =n-2; i >= 0; i--)
		x[i] = (b[i] - c[i]*x[i+1])/d[i];
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

static void heat_implicit(struct problem_spec *spec, double T, int n,
		int steps, char *gv_filename)
{
	FILE *fp;
	double *u, *v, *d, *c;
	double dx = (spec->b - spec->a)/(n+1);
	double dt =  T/steps;
	double r = dt/(dx*dx);
	if ((fp = fopen(gv_filename, "w")) == NULL) {
		fprintf(stderr, "unable to open file '%s' for writing \n", gv_filename);
		return;
	}

	fprintf(fp, "# geomview script written by the function %s()\n",
			__func__);
	fprintf(fp, "MESH %d %d\n", n+2, steps+1);
	printf("%g < x < %g, 0<t<%g, dx =%g, dt =%g, "
			"r = dt/(dx*dx) = %g\n", spec->a, spec->b, 
			T, dx, dt, r);

	make_vector(u, n+2);
	make_vector(v, n+2);
	make_vector(d, n);
	make_vector(c, n-1);

	for (int j = 0; j < n+2; j++) {
		double x = spec->a + (spec->b -spec->a)/(n+1)*j;
		u[j] = spec->ic(x);
	}

	plot_curve(fp, u ,n, steps, 0);

	for (int j = 0; j < n-1; j++)
		c[j] = -r;

	for (int k = 1; k <= steps; k++) {
		double *tmp;
		double t = T*k/steps;
		v[0] = spec->bcL(t);
		v[n+1] = spec->bcR(t);
		u[1] += r*v[0];
		u[n] += r*v[n+1];

		for (int i = 0; i < n; i++)
			d[i] = 1 + 2*r;

		trisolve(n, c, d, c, u+1, v+1);
		tmp = v;
		v = u;
		u = tmp;
		plot_curve(fp, u , n , steps, k);
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
	free_vector(d);
	free_vector(c);
	putchar('\n');
}

int main(int argc, char **argv)
{
	struct problem_spec *heat1(void);
	struct problem_spec *heat2(void);
	//struct problem_spec *heat3(void);
	//struct problem_spec *heat4(void);
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

	heat_implicit(heat1(), T, n, steps, "im1.gv");
	heat_implicit(heat2(), T, n, steps, "im2.gv");
	//heat_implicit(heat3(), T, n, steps, "im3.gv");
	//heat_implicit(heat4(), T, n, steps, "im4.gv");
	return EXIT_SUCCESS;
}

