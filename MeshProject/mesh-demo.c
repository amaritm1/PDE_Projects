#include <stdio.h>
#include <stdlib.h>
#include "problem-spec.h"
#include "mesh-to-eps.h"
#include "mesh.h"

void show_usage(char *progname)
{
	fprintf(stderr, "Usage: %s a\n", progname);
	fprintf(stderr, "    a = maximal triangle area\n");
}
/*
static void do_demo(struct problem_spec *spec, double a, char *eps_filename)
{
	struct mesh *mesh = make_mesh(spec,a);
	printf("vertices = %d, edges = %d, elems = %d\n",
			mesh->nnodes, mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, eps_filename);
	free_mesh(mesh);
}
*/	
int main(int argc, char **argv)
{

	struct problem_spec *triangle_with_hole(void);
	struct problem_spec *square(void);
	struct problem_spec *three_holes(int n);
	struct problem_spec *annulus(int n);
	void   free_annulus(struct problem_spec *spec);
	void   free_three_holes(struct problem_spec *spec);
	struct problem_spec *spec;
	struct mesh *mesh;
	double a;
	char *endptr;
	
	a = strtod(argv[1], &endptr); //converts the string imputed to a double
	
	if (argc != 2) {
		show_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	
	if ( *endptr != '\0' || a < 0.0) {
		show_usage(argv[0]);
		exit(EXIT_FAILURE);
	}
	
	printf("a = %g \n",a);
	
	spec = triangle_with_hole();
	mesh = make_mesh(spec, a);
	printf("mesh with %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, "triangle-with-hole.eps");
	free(mesh);
	
	spec = annulus(12);
	mesh = make_mesh(spec, a);
	printf("mesh with %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, "annulus.eps");
	free_mesh(mesh);
	free_annulus(spec);

	spec = three_holes(12);
	mesh = make_mesh(spec, a);
	printf("mesh with %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, "three_holes.eps");
	free_mesh(mesh);
	free_three_holes(spec);

	spec = square();
	mesh = make_mesh(spec, a);
	printf("mesh with %d nodes, %d edges, %d elements\n", mesh->nnodes,
			mesh->nedges, mesh->nelems);
	mesh_to_eps(mesh, "square.eps");
	free_mesh(mesh);

	return 0;
}

