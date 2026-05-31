#include <stdio.h>
#include "twb-quad.h"
#include "problem-spec.h"

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

	spec = triangle_with_hole();


