#include <stdio.h>
#include <stdlib.h>
#include "twb-quad.h"
#include "problem-spec.h"
#include "mesh.h"
#include "plot-with-geomview.h"

double integrate_over_triangle(struct elem *ep, struct TWB_qdat *qdat,
		double (*f)(double x, double y)) 
{
	double x[3], y[3]; 
	for(int i = 0; i < 3; i++) {
		x[i] = ep->n[i]->x;
		y[i] = ep->n[i]->y;
	}

	double sum = 0.0;

	while(qdat->weight != -1.0) {
		double lambda1 = qdat->lambda1;
		double lambda2 = qdat->lambda2;
		double lambda3 = qdat->lambda3;
		double X = lambda1*x[0] + lambda2*x[1] + lambda3*x[2];
		double Y = lambda1*y[0] + lambda2*y[1] + lambda3*y[2];
		sum += (qdat->weight)*f(X,Y);
		qdat++;
	}

	sum *= ep->area / TWB_STANDARD_AREA;

	return sum;
}

static void eval_f(struct mesh *mesh, double (*f)(double x, double y))
{
	for(int i = 0; i < mesh->nnodes; i++)
		mesh->nodes[i].z = f(mesh->nodes[i].x,mesh->nodes[i].y);
}		


void do_demo(struct problem_spec *spec, double a,
		struct TWB_qdat *qdat, char *gv_filename) 
{
	struct mesh *mesh = make_mesh(spec, a);

	printf("mesh of %d nodes, %d edges, and %d elements. \n",
			mesh->nnodes, mesh->nedges, mesh->nelems);

	double sum = 0.0;
	for(int i = 0; i < mesh->nelems; i++) 
		sum += integrate_over_triangle(&mesh->elems[i], qdat, spec->f);

	printf("The integral is %.12f\n", sum);

	eval_f(mesh, spec->f);
	plot_with_geomview_mono(mesh, gv_filename);
	/* output to 3D picture by putting z = f(x,y) at each node structure
	   in this mesh object. Do this before freeing mesh!
	 */
	free_mesh(mesh);
}

void show_usage_and_exit(char *progname) {
	fprintf(stderr, "Usage: %s da\n", progname);
	fprintf(stderr, "    d = twb integration dtrenth\n");
	fprintf(stderr, "    a = maximal triangle area\n");
	exit(EXIT_FAILURE);
}

int main( int argc, char **argv) {
	int d, n;
	double a;
	char *endptr;
	struct problem_spec *spec;
	struct problem_spec *triangle_with_hole(void);
	struct problem_spec *annulus(int n);
	struct problem_spec *square(void);
	struct problem_spec *three_holes(int n);
	void free_annulus(struct problem_spec *spec);
	void free_three_holes(struct problem_spec *spec);


	if (argc != 3)
		show_usage_and_exit(argv[0]);

	d = strtol(argv[1], &endptr, 10);
	if (*endptr != '\0')
		show_usage_and_exit(argv[0]);

	a = strtod(argv[2], &endptr);
	if (*endptr != '\0' || a <= 0.0)
		show_usage_and_exit(argv[0]);

	/* if twb_qdat() receives a d not an availible strength,
	   increments d until the next availible is chosen.
	 */
	struct TWB_qdat *qdat = twb_qdat(&d, &n);

	printf("Integrating with strength %d and %d points \n", d, n);

	spec = triangle_with_hole();
	do_demo(spec, a, qdat, "triangle_with_hole.gv");

	spec = annulus(12);
	do_demo(spec, a, qdat, "annulus.gv");
	free_annulus(spec);

	spec = square();
	do_demo(spec, a, qdat, "square.gv");

	spec = three_holes(12);
	do_demo(spec, a, qdat, "three_holes.gv");
	free_three_holes(spec);
	return 0;
}

