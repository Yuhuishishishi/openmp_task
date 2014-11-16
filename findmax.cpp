#include "Findmax.h"
#include <omp.h>
#include <cstdio>

void max_func(double(*func)(double), double a, double b, double eps, double s, double& max_x, double& max_val) {
	double start_time = omp_get_wtime();
	
	double fa = func(a);
	double fb = func(b);
	if (fa > fb) {
		max_val = func(a);
		max_x = a;
	} else {
		max_val = func(b);
		max_x = b;
	}

	omp_set_nested(1);
	#pragma omp parallel shared(max_val, max_x)
	{
		#pragma omp single 
		{
			printf("Running on %d threads.\n", omp_get_num_threads());
			eval_interval(func, a, b, fa, fb, eps, s, max_x, max_val);
		}
	}

	printf("Elapsed time: %.4f\n", omp_get_wtime() - start_time);

}

void eval_interval(double(*func)(double), double a, double b, double fa, double fb, double eps, double s, double& max_x, double& max_val) {
	// update the current max value
	if (fa > max_val || fb > max_val) {
		if (fa > fb) {
			max_val = fa;
			max_x = a;
		}
		else {
			max_val = fb;
			max_x = b;
		}
	}

	printf("int(%.3f, %.3f), max_val = %.3f, thread = %d\n", a, b, max_val, omp_get_thread_num());

	double possible_max_val = 0.5 * (fa + fb + s*(b - a));
	if (possible_max_val > max_val + eps) {
		double mid = 0.5*(b + a);
		double fmid = func(mid);
		// split the interval into two sub intervals
		#pragma omp task shared(max_val, max_x)
		eval_interval(func, a, mid, fa, fmid, eps, s, max_x, max_val);

		#pragma omp task shared(max_val, max_x)
		eval_interval(func, mid, b, fmid, fb, eps, s, max_x, max_val);
	}

	#pragma omp taskwait
}