#include <stdio.h>
#include "array.h"
#include "nelder-mead.h"
#define EXPAND 2.0
#define REFLECT 1.0
#define CONTRACT .5
#define SHRINK .5

static inline void rank_vertices(double *y, int m, int *ia, int *iy, int *iz)
{
	int min = 0;
	int mid = 0;
	int max = 1;

	for(int i = 0; i < m; i++) {
		if (y[i] < y[min]){
			min = i;
		} else if (y[i] >= y[max]) {
			max = i;
		}
	}
	mid = min;
	for(int j = 0; j < m; j++) {
		if (y[j] >= y[mid] && j != max ){
			mid = j;
		}
	}
	*iz = max;
	*iy = mid;
	*ia = min;
}


static void get_centroid(double **s, int n, int iz, double *C)
{
	for (int j = 0; j < n; j++) {
		double sum = 0.0;
		for (int i = 0; i < n + 1; i++) {
			if (i != iz) {
				sum += s[i][j];
			}
		}
		C[j] = sum/n;
	}
}

static inline void transform(double *P, double *Q, int n,
		double beta, double *R)
{
	for (int i = 0; i < n; i++) {
		R[i] = (1 - beta)*P[i] + beta*Q[i];
	}
}

static void shrink(double **s, int n, int ia)
{
	for (int i = 0; i < n + 1; i++)
		if (i != ia)
			transform(s[ia], s[i], n, SHRINK, s[i]);
}

static inline void replace_row(double **s, int i, double **r)
{
	double *tmp = s[i];
	s[i] = *r;
	*r = tmp;
}
static int done(double **s, int n, double *y, int ia, int iz, double err2)
{
	double norm = 0;
	double ymag = y[iz]-y[ia];
	for (int j = 0; j < n; j++)
		norm += (s[iz][j]-s[ia][j])*(s[iz][j]-s[ia][j]);
	if (ymag < 0)
		ymag = -ymag;
	return (norm <= err2) && (ymag <= err2);
}
int nelder_mead(struct nelder_mead *nm)
{
	double **s = nm->s;
	int n = nm->n;
	double h = nm->h;
	double tol = nm->tol;
	double err2 = (h*tol)*(h*tol);
	double *y, *C, *Pr, *Pe, *Pc;
	double yr, ye, yc;
	int ia, iy, iz;
	int simplex_to_be_freed = 0;
	int fevalcount;
	int i;

	make_vector(y, n+1); // vertex values
	make_vector(Pr, n);  // the reflexed points x^(r)
	make_vector(Pe, n);  // the expanded points x^(e)
	make_vector(Pc, n);  // the contracted points x^(c)
	make_vector(C, n);   // centroid of the face opposite the vertex iz

	if (s == NULL) {
		make_matrix(s, n+1, n);

		for (int k = 0; k < n; k++)
			s[0][k] = nm->x[k];
		for (int i = 1; i < n+1; i++){
			for (int j = 0; j < n; j++)
				s[i][j] = nm->x[j];
			s[i][i-1] += h;
		}
		simplex_to_be_freed = 1;
	}

	for (i = 0; i < n + 1; i++)
		y[i] = nm->f(s[i], n, nm->params);
	fevalcount = n + 1;

	while (fevalcount <= nm->maxevals) {
		rank_vertices(y, n + 1, &ia, &iy, &iz);
		if (done(s, n, y, ia, iz, err2)) {
			nm->minval = y[ia];
			for (i = 0; i < n; i++) {
				nm->x[i] = s[ia][i];
			}
			// copy the best vertex into the vector nm -> x
			break;
		}
		get_centroid(s, n , iz, C);
		transform(C, s[iz], n, -REFLECT, Pr);
		yr = nm->f(Pr, n, nm->params);
		fevalcount++;

		if (yr <  y[ia]){                        //case 1
			transform(C, Pr, n, EXPAND, Pe);
			ye = nm->f(Pe, n, nm->params);
			fevalcount++;
			if (ye < yr){
				replace_row(s, iz, &Pe);
				y[iz] = ye;
			} else {
				replace_row(s, iz, &Pr);
				y[iz] = yr;
			}
		}
		else if (yr < y[iy]) {                     //case 2
			fevalcount++;
			replace_row( s, iz, &Pr);
		}
		else {                                     //case 3&4
			if (y[iy] <= yr && yr < y[iz]) {
				transform(C, Pr, n, CONTRACT, Pc);
				yc = nm->f(Pc, n, nm->params);
				fevalcount++;
				if (yc < yr){
					replace_row(s, iz, &Pc);
					y[iz] = yc;
				} else {
					shrink(s, n, ia);
					fevalcount++;
					for (i = 0; i < n; i++){
						if (i != ia)
							y[i] = nm->f(s[i], n, nm->params);
					}
				}
			} else {
				transform(C, Pr, n, -CONTRACT, Pc);
				yc = nm->f(Pc, n, nm->params);
				fevalcount++;
				if (yc < yr) {
					replace_row(s, iz, &Pc);
					y[iz] = yc;
				} else {
					shrink(s, n, ia);
					fevalcount++;
					for (i = 0; i < n; i++){
						if (i != ia)
							y[i] = nm->f(s[i], n, nm->params);
					}
				}
			}
		}
	}

	free_vector(y);
	free_vector(C);
	free_vector(Pr);
	free_vector(Pe);
	free_vector(Pc);

	if (simplex_to_be_freed)
		free_matrix(s);

	return fevalcount;
}
