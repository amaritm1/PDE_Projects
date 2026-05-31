#include <stdio.h>
#include "twb-quad.h"

double f(double x, double y)
{
	return x*x + y*y;

}

int main(void)
{
	double v1[]= { 0.0, 0.0 };
	double v2[]= { 1.0, 0.0 };
	double v3[]= { 0.0, 1.0 };
	int d = 2, n;
	struct TWB_qdat *qdat;
	qdat = twb_qdat(&d, &n);
	double a = 0.5;
	double sum = 0;

	for (int i = 0; i < n; i++) {
		double lambda1 = qdat[i].lambda1;
		double lambda2 = qdat[i].lambda2;
		double lambda3 = qdat[i].lambda3;
		double w = qdat[i].weight;
		double x = lambda1*v1[0] + lambda2*v2[0] + lambda3*v3[0];
		double y = lambda1*v1[1] + lambda2*v2[1] + lambda3*v3[1];
		sum += w*f(x,y);
		
	}

	sum *= a/TWB_STANDARD_AREA;
	printf("integrated with strength %d and number of points = %d\n", d, n);
	printf("integral = %f\n", sum);
return 0;
}
