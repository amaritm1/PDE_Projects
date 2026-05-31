#include <stdio.c>
#include <stdlib.h>
#include "problem-spec.h"
#include "mesh-to-eps.h"

void show_usage(char *progname)
{
	fprintf(stderr, "Usage: %s a\n", progname);
	fprintf(stderr, "    a = maximal triangle area\n");
}

int main(int argc. char **argv)
{

	struct problem_spec *triangle_with_hole(void);
	struct problem_spec *annulus(int n);
	void free_annulus(problem_spec *spec);
	struct problem_spec *spec;
	struct mesh *mesh;
	double a;
	char *endptr;
	
	if (argc != 2) {
		show_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	a = strtod(argv[1], &endptr); //converts the string imputed to a double
	
	if ( *endptr != '\0' || a <= 0.0) {
		show_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	spec = triangle_with_hole();
	make_mesh(spec, a);
	printf("mesh with %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, "triangle-with-hole.eps");
	free_mesh(mesh);
	
	spec = annulus(12);
	make_mesh(spec, a);
	printf("mesh with %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, "annulus.eps");
	free_mesh(mesh);
	free_annulus(spec);

	return 0;

}

