#include <stdio.h>
#include <math.h>
#include "problem-spec.h"
#include "array.h"

//TRIANGLE WITH HOLE--------------------------------------
double triangle_with_hole_f(double x, double y)
{
	return 30*x*y*(2-x)*(2-y);
}

double annulus_f(double x, double y)
{
	  double t = atan2(y,x);
	  return 20*cos(3*t);
}

double three_holes_f(double x, double y)
{
	  return 100*(x*x+y*y);
}

double square_f(double x, double y)
{
	  return 32*(x*(1-x) + y*(1-y));
}

double square_u_exact(double x, double y)
{
	  return 16*x*y*(1-x)*(1-y);
}

struct problem_spec *triangle_with_hole(void)
{
	static struct problem_spec_point points[] = {
		{ 0, -1.0, 0.0,   FEM_BC_DIRICHLET},
		{ 1,  1.0,  0.0,  FEM_BC_DIRICHLET},
		{ 2,  0.0,  2.0,  FEM_BC_DIRICHLET},
		{ 3, -0.25, 0.25, FEM_BC_DIRICHLET},
		{ 4,  0.25, 0.25, FEM_BC_DIRICHLET},
		{ 5,  0.25, 1.0,  FEM_BC_DIRICHLET},
		{ 6, -0.25, 1.0,  FEM_BC_DIRICHLET}
	};

	static struct problem_spec_segment segments[] = {
		{ 0, 0, 1,  FEM_BC_DIRICHLET },
		{ 1, 1, 2,  FEM_BC_DIRICHLET },
		{ 2, 2, 0,  FEM_BC_DIRICHLET },
		{ 3, 3, 4,  FEM_BC_DIRICHLET },
		{ 4, 4, 5,  FEM_BC_DIRICHLET },
		{ 5, 5, 6,  FEM_BC_DIRICHLET },
		{ 6, 6, 3,  FEM_BC_DIRICHLET }
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
		.f 		= triangle_with_hole_f,
		.g		= NULL,
		.h		= NULL,
		.eta		= NULL,
		.u_exact	= NULL,
	};

	printf("Triangle with a hole\n");

	return &spec;
}

struct problem_spec *square(void)
{
	static struct problem_spec_point points[] = {
		{ 0.0,  0.0, 0.0,  FEM_BC_DIRICHLET},
		{ 1.0,  1.0, 0.0,  FEM_BC_DIRICHLET},
		{ 2.0,  1.0, 1.0,  FEM_BC_DIRICHLET},
		{ 3.0,  0.0, 1.0,  FEM_BC_DIRICHLET},
	};

	static struct problem_spec_segment segments[] = {
		{ 0, 0, 1,  FEM_BC_DIRICHLET },
		{ 1, 1, 2,  FEM_BC_DIRICHLET },
		{ 2, 2, 3,  FEM_BC_DIRICHLET },
		{ 3, 3, 0,  FEM_BC_DIRICHLET },
	};

	static struct problem_spec spec = {
		.points 	= points,
		.segments	= segments,
		.holes   	= NULL,
		.npoints 	= (sizeof points) / (sizeof points[0]),
		.nsegments 	= (sizeof segments) / (sizeof segments[0]),
		.nholes 	= 0,
		.f 		= square_f,
		.g		= NULL,
		.h		= NULL,
		.eta		= NULL,
		.u_exact	= square_u_exact,
	};

	printf("Square \n");

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

	static struct problem_spec_hole *holes;
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
	spec->f = annulus_f;
	spec->g = NULL;
	spec->h = NULL;
	spec->eta = NULL;
	spec->u_exact = NULL;

	printf("An %d-sided annulus\n", n);

	return spec;

}

struct problem_spec *three_holes(int n)
{

	double Pi = 4.0*atan(1);
	double s = 0.64;
	double r = s/4;

	static struct problem_spec_point *points;
	make_vector(points, 3*n+6);

	points[0].point_no = 0;
	points[0].x  = 0;
	points[0].y  = 0;
	points[0].bc = FEM_BC_DIRICHLET;

	points[1].point_no = 1;
	points[1].x  = s;
	points[1].y  = 0;
	points[1].bc = FEM_BC_DIRICHLET;

	points[2].point_no = 2;
	points[2].x  = s;
	points[2].y  = s;
	points[2].bc = FEM_BC_DIRICHLET;

	points[3].point_no = 3;
	points[3].x  = -s;
	points[3].y  =  s;
	points[3].bc = FEM_BC_DIRICHLET;

	points[4].point_no = 4;
	points[4].x  = -s;
	points[4].y  = -s;
	points[4].bc = FEM_BC_DIRICHLET;

	points[5].point_no = 5;
	points[5].x  =  0;
	points[5].y  = -s;
	points[5].bc = FEM_BC_DIRICHLET;

	// First hole points
	for(int i = 6; i < (2*n+6); i++) {
	points[i].point_no = i;
	points[i].x  = (-s/2) + r*cos(2*Pi/n*(i-6));
	points[i].y  = (-s/2) + r*sin(2*Pi/n*(i-6));
	points[i].bc = FEM_BC_DIRICHLET;
	}

	//Second hole points
	for (int i = n+6; i < 2*n+6; i++) {
	points[i].point_no = i;
	points[i].x  = (-s/2) + r*cos(2*Pi/n*(i-6));
	points[i].y  = (s/2) + r*sin(2*Pi/n*(i-6));
	points[i].bc = FEM_BC_DIRICHLET;
	}

	// Thrid hole points
	for(int i = 2*n+6; i < (3*n+6); i++) {
	points[i].point_no = i;
	points[i].x  = (s/2) + r*cos(2*Pi/n*(i-6));
	points[i].y  = (s/2) + r*sin(2*Pi/n*(i-6));
	points[i].bc = FEM_BC_DIRICHLET;
	}

	static struct problem_spec_segment *segments;
	make_vector(segments, 3*n+6);
	segments[5].bc = FEM_BC_DIRICHLET;

	segments[0].segment_no = 0;
	segments[0].point_no_1 = 0;
	segments[0].point_no_2 = 1;
	segments[0].bc = FEM_BC_DIRICHLET;

	segments[1].segment_no = 1;
	segments[1].point_no_1 = 1;
	segments[1].point_no_2 = 2;
	segments[1].bc = FEM_BC_DIRICHLET;

	segments[2].segment_no = 2;
	segments[2].point_no_1 = 2;
	segments[2].point_no_2 = 3;
	segments[2].bc = FEM_BC_DIRICHLET;

	segments[3].segment_no = 3;
	segments[3].point_no_1 = 3;
	segments[3].point_no_2 = 4;
	segments[3].bc = FEM_BC_DIRICHLET;

	segments[4].segment_no = 4;
	segments[4].point_no_1 = 4;
	segments[4].point_no_2 = 5;
	segments[4].bc = FEM_BC_DIRICHLET;

	segments[5].segment_no = 5;
	segments[5].point_no_1 = 5;
	segments[5].point_no_2 = 0;
	segments[5].bc = FEM_BC_DIRICHLET;
	

	//First hole segments
	for(int i = 6; i < n+6; i++) {
		segments[i].segment_no = i;
		segments[i].point_no_1 = i;
		segments[i].point_no_2 = i+1;
		segments[i].bc  = FEM_BC_DIRICHLET;
	}

	//Second hole segments
	for(int i = n+6; i < (2*n+6); i++) {
		segments[i].segment_no = i;
		segments[i].point_no_1 = i;
		segments[i].point_no_2 = i+1;
		segments[i].bc  = FEM_BC_DIRICHLET;
	}

	//Third hole segments
	for(int i = 2*n+6; i < (3*n+6); i++) {
		segments[i].segment_no = i;
		segments[i].point_no_1 = i;
		segments[i].point_no_2 = i+1;
		segments[i].bc  = FEM_BC_DIRICHLET;
	}


	segments[n   + 6 - 1].point_no_2 -=n;
	segments[2*n + 6 - 1].point_no_2 -=n;
	segments[3*n + 6 - 1].point_no_2 -=n;



	static struct problem_spec_hole *holes;
	make_vector(holes, 3);
	holes[0].x = -s/2;
	holes[0].y = -s/2;

	holes[1].x = -s/2;
	holes[1].y =  s/2;

	holes[2].x =  s/2;
	holes[2].y =  s/2;

	static struct problem_spec *spec;
	spec = xmalloc(sizeof *spec);


	spec->points 	= points;
	spec->segments	= segments;
	spec->holes   	= holes;
	spec->npoints 	= 3*n+6;
	spec->nsegments = 3*n+6;
	spec->nholes 	= 3;
	spec->f 	= three_holes_f;
	spec->g		= NULL;
	spec->h		= NULL;
	spec->eta	= NULL;
	spec->u_exact	= NULL;

	printf("A L-shape with three holes\n");

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

void free_three_holes(struct problem_spec *spec)
{
	if (spec != NULL){
		free_vector(spec->points);
		free_vector(spec->segments);
		free_vector(spec->holes);
		free(spec);
	}
}
