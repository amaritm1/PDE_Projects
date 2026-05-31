#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <suitesparse/umfpack.h>
#include "poisson.h"
#include "problem-spec.h"
#include "triangle.h"
#include "twb-quad.h"
#include "array.h"
#include "mesh.h"

static void error_and_exit(int status, const char *file, int line)
{
	fprintf(stderr, "Usage: %s da\n", file);
	fprintf(stderr, "    d = twb integration strenth\n");
	fprintf(stderr, "    a = maximal triangle area\n");
	exit(EXIT_FAILURE);
}

static void enforce_zero_dirichlet_bc(struct elem *ep, double k[3][4])
{
	for(int i=0; i<3; i++){
		if (ep->n[i]->bc == FEM_BC_DIRICHLET){
			for(int j=0; j<3; j++)
				k[i][j]=k[j][i]=0;
			k[i][i]=1;
			k[i][3]=0;
		}
	}
}

static void compute_element_stiffness(struct elem *ep, struct TWB_qdat *qdat,
		double (*f)(double x, double y), double k[3][4])
{
	double x[3];
	double y[3];

	for(int i = 0; i < 3; i++) {
		x[i] = ep->n[i]->x;
		y[i] = ep->n[i]->y;
	}
	
	for(int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++){
			k[i][j] = fabs(1.0/(4.0*ep->area));
			k[i][j] *= (ep->ex[i])*(ep->ex[j]) 
				+(ep->ey[i])*(ep->ey[j]);
		}
	}

	for (int i = 0; i < 3; i++)
		k[i][3] = 0;

	while(qdat->weight != -1.0) {
		double lambda[3];
		lambda[0] = qdat->lambda1;
		lambda[1] = qdat->lambda2;
		lambda[2] = qdat->lambda3;
		double X = lambda[0]*x[0] + lambda[1]*x[1] + lambda[2]*x[2];
		double Y = lambda[0]*y[0] + lambda[1]*y[1] + lambda[2]*y[2];
		for (int i = 0; i < 3; i++)
			k[i][3] += qdat-> weight * f(X,Y) * lambda[i];
		qdat++;
	}
	for(int i = 0; i < 3; i++)
		k[i][3] *= ep-> area / TWB_STANDARD_AREA;
}	
void poisson_solve(struct problem_spec *spec, struct mesh *mesh, int d)
{
	double k[3][4];
	int *Ti, *Tj, *Ai, *Ap;
	double *Tx, *Ax, *F, *U;
	int status;
	void *Symbolic = NULL;
	void *Numeric = NULL;
	int i, j, r, s;
	struct TWB_qdat *qdat = twb_qdat(&d, NULL);

	make_vector(Ti, 3*3*mesh->nelems);
	make_vector(Tj, 3*3*mesh->nelems);
	make_vector(Tx, 3*3*mesh->nelems);

	make_vector(Ap, 1 + mesh->nelems);
	make_vector(Ai, 3*3*mesh->nelems);
	make_vector(Ax, 3*3*mesh->nelems);

	make_vector(F, mesh->nnodes);
	make_vector(U, mesh->nnodes);

	for (i = 0; i < mesh->nnodes; ++i)
		F[i] = 0.0;

	s = 0;
	for (r = 0; r < mesh->nelems; ++r) {
		struct elem *ep = &mesh->elems[r];
		compute_element_stiffness(ep, qdat, spec->f, k);
		enforce_zero_dirichlet_bc(ep, k);
		for (i = 0; i < 3; ++i) {
			int I = ep->n[i]->nodeno;
			for (j = 0; j < 3; ++j) {
				if (k[i][j] != 0.0) {
					int J = ep->n[j]->nodeno;
					Ti[s] = I;
					Tj[s] = J;
					Tx[s] = k[i][j];
					++s;
				}
			}
			F[I] += k[i][3];
		}
	}
	
	status = umfpack_di_triplet_to_col(mesh->nnodes, mesh->nnodes, s,
			Ti, Tj, Tx, Ap, Ai, Ax, NULL);
	if (status != UMFPACK_OK)
		error_and_exit(status, __FILE__, __LINE__);

	printf("system stiffness matrix is %dx%d (=%d) "
			"has %d nonzero entries\n",
			mesh->nnodes, mesh->nnodes,
			mesh->nnodes * mesh->nnodes,
			Ap[mesh->nnodes]);
	//symbolic analysis
	status = umfpack_di_symbolic(mesh->nnodes, mesh->nnodes, Ap, Ai, Ax,
			&Symbolic, NULL, NULL);
	if (status != UMFPACK_OK)
		error_and_exit(status, __FILE__, __LINE__);


	//numeric analysis
	status = umfpack_di_numeric(Ap, Ai, Ax, Symbolic, &Numeric, NULL, NULL);
	if (status != UMFPACK_OK)
		error_and_exit(status, __FILE__, __LINE__);

	//solve the system
	status = umfpack_di_solve(UMFPACK_A, Ap, Ai, Ax, U, F, Numeric, NULL, NULL);
	if (status != UMFPACK_OK)
		error_and_exit(status, __FILE__, __LINE__);

	for (i = 0; i < mesh->nnodes; i++)
		mesh->nodes[i].z = U[i];

	free_vector(Ti);
	free_vector(Tj);
	free_vector(Tx);
	free_vector(Ap);
	free_vector(Ai);
	free_vector(Ax);
	free_vector(F);
	free_vector(U);
	umfpack_di_free_symbolic(&Symbolic);
	umfpack_di_free_numeric(&Numeric);

}

static struct errors elem_errors(struct problem_spec *spec, struct TWB_qdat
		*qdat, struct elem *ep)
{
	struct errors elem_errors = {0.0, 0.0, 0.0};

	double x[3], y[3];
	double fem_u = 0.0;
	double exact_u = 0.0;

	for(int i = 0; i < 3; i++) {
		x[i] = ep->n[i]->x;
		y[i] = ep->n[i]->y;
	}

	while(qdat->weight != -1.0) {
		double lambda[3];
		lambda[0] = qdat->lambda1;
		lambda[1] = qdat->lambda2;
		lambda[2] = qdat->lambda3;
		double X = lambda[0]*x[0] + lambda[1]*x[1] + lambda[2]*x[2];
		double Y = lambda[0]*y[0] + lambda[1]*y[1] + lambda[2]*y[2];

		exact_u = spec->u_exact(X, Y);

		for (int i=0; i<3; i++)
			fem_u += lambda[i] * ep->n[i]->z;

		elem_errors.L2norm += 
			qdat->weight*(exact_u - fem_u)*(exact_u - fem_u);

		elem_errors.energy += qdat->weight*
			(exact_u - fem_u)* spec->f(X,Y);

		if(fabs(exact_u - fem_u) > elem_errors.Linfty)
			elem_errors.Linfty = fabs(exact_u - fem_u);

		qdat++;
		fem_u = 0;
	}

	elem_errors.L2norm *= fabs(ep->area/TWB_STANDARD_AREA);
	elem_errors.energy *= fabs(ep->area/TWB_STANDARD_AREA);

	return elem_errors;
}


struct errors eval_errors(struct problem_spec *spec, struct mesh *mesh, int d)
{
	struct errors errs;
	struct TWB_qdat *qdat = twb_qdat(&d, NULL);
	errs.Linfty = errs.L2norm =errs.energy = 0.0;
	for (int i = 0; i < mesh->nelems; i++){
		struct elem *ep = &mesh->elems[i];
		struct errors elem_errs = elem_errors(spec,qdat,ep);
		errs.L2norm += elem_errs.L2norm;
		errs.energy += elem_errs.energy;

		if (elem_errs.Linfty > errs.Linfty)
			errs.Linfty = elem_errs.Linfty;
	}
	errs.L2norm = sqrt(errs.L2norm);
	errs.energy = sqrt(errs.energy);
	return errs;
}
