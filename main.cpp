#include <cstdio>
#include <math.h>
#include "Findmax.h"
#include <cstdlib>
#include <omp.h>

double f(double); // function to evaluate

int main(int argc, char* argv[]) {
	printf("----------------------------Program Start----------------------\n");
	// parse the arguments
	double a = 1, b = 100, eps = 1e-6, s = 12;
	int num_thread;
	if (argc > 1) {
		a = atof(argv[1]);
		b = atof(argv[2]);
		eps = atof(argv[3]);
		s = atof(argv[4]);
		num_thread = atoi(argv[5]);
	}

	// omp environment settings
	//omp_set_nested(1);
	//omp_set_num_threads(num_thread);
	printf("Running on %d threads\n", omp_get_num_threads());

	double max_x, max_val; // variables to store the max values
	max_func(f, a, b, eps, s, max_x, max_val);
	printf("max_val = %.3f, x = %.3f\n", max_val, max_x);

	return 0;
}

double f(double x) {
	double f_val = 0, inner_sum;
	for (int i = 100; i > 0; i--) {
		inner_sum = 0;
		for (int j = i; j > 0; j--) {
			inner_sum += pow(x + j, -3.1);
		}
		f_val += sin(x + inner_sum) / pow(1.2, i);
	}
	return f_val;
}