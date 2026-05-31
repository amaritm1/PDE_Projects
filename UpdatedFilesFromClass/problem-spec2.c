#include <stdio.h>
#include <math.h>
#include "problem-spec.h"
#include "array.h"

//TRIANGLE WITH HOLE-------------------------------------- 
struct problem_spec *triangle_with_hole(void)
{

	static struct problem_spec_point points[] = {

		{ 0, -1.0, 0.0, FEM_BC_DIRICHLET};
		{ 1, 1.0,  0.0, FEM_BC_DIRICHLET};
		{ 2, 0.0,  2.0, FEM_BC_DIRICHLET};
		{ 3, -0.25, 0.25, FEM_BC_DIRICHLET};
		{ 4, 0.25, 0.25, FEM_BC_DIRICHLET};
		{ 5, 0.25, 1.0, FEM_BC_DIRICHLET};
		{ 6, -0.25, 1.0, FEM_BC_DIRICHLET};
	};

	static struct problem_spec_segment segments[] = {
		{ 0, 0, 1,  FEM_BC_DIRICHLET },
		{ 1, 1, 2,  FEM_BC_DIRICHLET },
		{ 2, 2, 0,  FEM_BC_DIRICHLET },
		{ 3, 3, 4,  FEM_BC_DIRICHLET },
		{ 5, 5, 6,  FEM_BC_DIRICHLET },
		{ 6, 6, 3,  FEM_BC_DIRICHLET },

	};

	static struct problem_spec_hole holes[] = {
		{ 0.0, 0.75 },

	};

	static struct problem_spec spec = {
		.points 	= points,
		.segments	= segments,
		.holes 		= holes,
		.npoints 	= (sizeof points) / (sizeof points[0]),
		.nsegments 	= (sizeof segments) / (sizeof segments[0]),
		.nholes 	= (sizeof holes) / (sizeof holes[0]),
		.f 		= NULL,
		.g		= NULL,
		.h		= NULL,
		.eta		= NULL,
		.u_exact	= NULL,

	};

	printf("Triangle with a hole\n");

	return &spec;

}



struct problem_spec *annulus(int n)
{

	struct problem_spec_point *points;
	double Pi = 4.0*atan(1);
	double a = 0.325;
	double b = 2*a;

	make_vector(points, 2*n);

	for (int i = 0; i < n; i++) {
		points[i].point_no = i;
		points[i].x  = a*cos(2*Pi/n*i);
		points[i].y  = a*sin(2*Pi/n*i);
		points[i].bc = FEM_BC_DIRICHLET;
	}

	for (int i = 0; i < n; i++) {
		points[n+i].point_no = n + i;
		points[n+i].x  = b*cos(2*Pi/n*i);
		points[n+i].y  = b*sin(2*Pi/n*i);
		points[n+i].bc = FEM_BC_DIRICHLET;
	}

	static struct problem_spec_segment *segments;
	make_vector(segments, 2*n);

	for(int i = 0; i < n; i++) {
		segments[i].segment_no = i;
		segments[i].point_no_1 = i;
		segments[i].point_no_2 = i+1;
		segments[i].bc  = FEM_BC_DIRICHLET;
	}	

	for(int i = 0; i < n; i++) {
		segments[n+i].segment_no = n+i;
		segments[n+i].point_no_1 = n+i;
		segments[n+i].point_no_2 = n+i+1;
		segments[n+i].bc  = FEM_BC_DIRICHLET;
	}

	segments[n-1].point_no_2 -= n;
	segments[2*n-1].point_no_2 -= n;

	struct problem_spec_holes *holes;
	make_vector(holes, 1);
	holes[0].x = 0.0;
	holes[0].y = 0.0;

	struct problem_spec *spec;	
	spec = xmalloc(sizeof *spec);

	spec->points = points;
	spec->segments = segments;
	spec->holes = holes;
	spec->npoints = 2*n;
	spec->nsegments = 2*n;
	spec->nholes = 1;
	spec->f = NULL;
	spec->g = NULL;
	spec->h		= NULL;
	spec->eta		= NULL;
	spec->u_exact	= NULL;

	printf("An %d-sided annulus\n, n");

	return spec;

}

void free_annulus(struct problem_spec *spec)
{
	if (spec != NULL){
		free_vector(spec->points);
		free_vector(spec->segments);
		free_vector(spec->holes);
		free(spec);
	}
}
