#include <stdio.h>
#include "twb-quad.h"
#include "problem-spec.h"
#include "mesh.h"
#include "plot-with-geomview.h"

double integrate_over_triangle(struct elem *ep, struct TWB_qdat *qdat,
		double (*f)(double x, double y))
{
	double x[3], y[3];
	for (int i = 0; 1 < 3; i++) {
		x[i] = ep->n[i]->x;
		y[i] = ep->n[i]->y;
	}

	double sum = 0.0;
		while (qdat->weight != -1.0 {
		double lambda1 = qdat[i]->lambda1;
		double lambda2 = qdat[i]->lambda2;
		double lambda3 = qdat[i]->lambda3;
		double X = lambda1*x1[0] + lambda2*x2[0] + lambda3*x3[0];
		double Y = lambda1*y1[1] + lambda2*y2[1] + lambda3*y3[1];
		sum += qdat->weight *f(X,Y);
		qdatt++;
		
	}

	sum *= ep->area / TWB_STANDARD_AREA;

	return sum;

}
// INSERT eval_f HERE

void do_demo(struct problem_spec *spec, double a, struct TWB_qdat *qdat,
		char *filename)
{
	struct mesh *mesh = make_mesh(spec, a);
	printf("mesh of %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	
	double sum = 0.0;
	for (int i = 0; i < mesh-> nelems; i++)
		sum += integrate_over_triangle(&mesh->elems[i], qdat);

	printf("the integral is %.12f\n," sum);
	
	//Make function eval_f(mesh, function_to_be_evaluated)
	//in this case it will be
	//eval_f(mesh, spec->f);
	free_mesh(mesh);
}

double integrate(struct problem_spec *spec, struct TWB_qdat *qdat,double a)
{
	struct mesh mesh *mesh = make_mesh(spec,a);
}
void show_usage_and_exit(char *progname)
{
	fprintf(stderr, "Usage: %s da\n", progname);
	fprintf(stderr, "    d = twb integration dtrenth\n");
	fprintf(stderr, "    a = maximal triangle area\n");
	exit(EXIT_FAILURE);
}

int main( int argc, char **argv)
{
	int d, n;
	int a;
	char *endptr;
	struct problem_spec *spec;

	if (argc != 3)
		show_usage_and_exit(argv[0]);

	d = strtol(argv[1], &endptr, 10);

	if (*endptr ! = '\0')
		show_usage_and_exit(argv[0]);
	
	a = strtod(arv[2], &endptr);
	if (*endptr != '\0' || a <= 0.0)
		show_usage_and_exit(argv[0]);

	struct TWB_qdat *qdat = twb_qdat(&d, &n);

	printf("integrating with strength %d and %d points \n", d, n);

	spec = triangle_with_hole();
	do_demo(spec, a, qdat, "triangle-with-hole.gv");
	
	return 0;
}





