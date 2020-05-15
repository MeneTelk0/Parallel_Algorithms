#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <stddef.h>
//Keywords: Integral OpenMP precision recursive parallel trapezoid method

// In this problem, the trapezoid method is used to calculate the value 
// of the integral with a given precision with use of OpenMP

// Choose threshold to reach the best perfomance
// For integral in this example it looks like the best one
#define THRESHOLD 0.0000005


// Declare a function and its scope
// Declare values of [A,B]
// Function in this example:
// https://www.wolframalpha.com/input/?i=integrate+1%2Fx%5E2+sin%281%2Fx%29%5E2+from+0.0001+to+1
const double A = 0.0001;
const double B = 1;

double function(double x)
{
	double s = sin(1.0 / x ) / x ;
    return s*s;
}

double integral_OMP_serial(double, double, double, double, double);
double integral_OMP_parallel(double, double, double, double, double);

// Function for starting parallel section
double integral_OMP(double a, double b, double fa, double fb, double eps)
{
	double integral = 0.0;
	// Opening parallel section
	#pragma omp parallel
		{   // Single is used to call integral_OMP_parallel() only once
			#pragma omp single nowait
			integral = integral_OMP_parallel(a, b, fa, fb, eps);
		}
	return integral;
}

// Main parallel module in the program
double integral_OMP_parallel(double a, double b, double fa, double fb, double eps)
{
	double I = 0.0; // Integral value on the each step
	double c = (a + b) / 2.0;  // Middle of the [a,b]
	double fc = function(c);
	double sab = (fa + fb) * (b - a) / 2.0; // Square of trapezoid [a,b]
	double sac = (fa + fc) * (c - a) / 2.0; // Square of trapezoid [a,c]
	double scb = (fc + fb) * (b - c) / 2.0; // Square of trapezoid [c,b]
	double sabc = sac + scb;
	double leftI, rightI; // Values of Integral for [a,c] and [c,b], respectively
	if (fabs(sab - sabc) >= eps * fabs(sabc)) { // Checking if we meet precision requirements (epsilon)
		if (fabs(b - a) < THRESHOLD){           // Decide -> Parallel or Serial section
			// Serial section
			leftI = integral_OMP_serial(a, c, fa, fc, eps);
			rightI = integral_OMP_serial(c, b, fc, fb, eps);

			I = leftI + rightI;
		} else {
			// Parallel section
			#pragma omp task shared(leftI)  // Creates pull of tasks for the kernels
			{
				leftI = integral_OMP_parallel(a, c, fa, fc, eps);
			}
			rightI = integral_OMP_parallel(c, b, fc, fb, eps);

			#pragma omp taskwait           // Waits until all generated tasks will finish
			I = leftI + rightI;
		}
	} else {  // If precision requirements are met, then using sabs as integral value
		I = sabc;
	}
	return I;
}

// Serial module in the program
double integral_OMP_serial(double a, double b, double fa, double fb, double eps)
{
	double I = 0.0; // Integral value on the each step
	double c = (a + b) / 2.0;  // Middle of the [a,b]
	double fc = function(c);
	double sab = (fa + fb) * (b - a) / 2.0; // Square of trapezoid [a,b]
	double sac = (fa + fc) * (c - a) / 2.0; // Square of trapezoid [a,c]
	double scb = (fc + fb) * (b - c) / 2.0; // Square of trapezoid [c,b]
	double sabc = sac + scb;
	if (fabs(sab - sabc) >= eps * fabs(sabc)) { // Checking if we meet precision requirements (epsilon)
		I = integral_OMP_serial(a, c, fa, fc, eps) + integral_OMP_serial(c, b, fc, fb, eps);
	} else {  // If precision requirements are met, then using sabs as integral value
		I = sabc;
	}
	return I;
}

int main(int argc, char* argv[])
{
	// Defined precision value (epsilon)
	double eps = 1e-6;
	// Input value from CLI
	if (argc > 1) {
		eps = atof(argv[1]);
	}
	// Defined value to calculate the average program run time in a few starts
	int num_of_starts = 5;
	
	double integral;
	// Variables to store time
	double start = omp_get_wtime();
	for (int i = 0; i < num_of_starts; i++){
		integral = integral_OMP(A, B, function(A), function(B), eps);
	}
	double end = omp_get_wtime();

	//Output to see that everything works great
	//printf("Integral value: %f, Average working time: %f\n",integral, (end-start)/num_of_starts);

	//Output to print plots with GNUPlot
	printf("%f\n", (end-start)/num_of_starts);
	
	return EXIT_SUCCESS;
}
