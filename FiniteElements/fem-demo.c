#include <stdio.h>
#include <stdlib.h>
#include "problem-spec.h"
#include "poisson.h"
#include "mesh.h"
#include "plot-with-geomview.h"

void do_demo(struct problem_spec *spec, double a, int d, char* gv_filename) 
{
	struct mesh *mesh = make_mesh(spec, a);

	printf("mesh of %d nodes, %d edges, and %d elements. \n",
			mesh->nnodes, mesh->nedges, mesh->nelems);

	poisson_solve(spec, mesh, d);
	if (spec->u_exact != NULL){
		struct errors errors = eval_errors(spec, mesh, d);
		printf("error: L^infty = %g, L^2 = %g, energy norm = %g\n.",
			errors.Linfty, errors.L2norm, errors.energy);
	}

	
	plot_with_geomview_zhue(mesh, gv_filename);
	/* output to 3D picture by putting z = f(x,y) at each node structure
	   in this mesh object. Do this before freeing mesh!
	 */
	free_mesh(mesh);
}

void show_usage_and_exit(char *progname) {
	fprintf(stderr, "Usage: %s da\n", progname);
	fprintf(stderr, "    d = twb integration strenth\n");
	fprintf(stderr, "    a = maximal triangle area\n");
	exit(EXIT_FAILURE);
}

int main( int argc, char **argv) {
	int d;
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

	spec = triangle_with_hole();
	do_demo(spec, a, d, "triangle_with_hole.gv");

	spec = annulus(12);
	do_demo(spec, a, d, "annulus.gv");
	free_annulus(spec);

	spec = square();
	do_demo(spec, a, d, "square.gv");

	spec = three_holes(12);
	do_demo(spec, a, d, "three_holes.gv");
	free_three_holes(spec);

	return 0;
}
