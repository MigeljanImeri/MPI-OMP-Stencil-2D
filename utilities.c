#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utilities.h"
//code from manning textbook
double** malloc2D(int jmax, int imax) {

	double** x = (double** ) malloc(jmax * sizeof(double *) + jmax * imax * sizeof(double));

	//zeroes out array
	memset(x,0,jmax * sizeof(double *) + jmax * imax * sizeof(double));

	x[0] = (double *)x + jmax;

	for (int j = 1; j < jmax; j++) {
		x[j] = x[j - 1] + imax;
	}

	return (x);
}
//code from manning textbook
void cpu_timer_start(struct timespec *tstart_cpu) {
   clock_gettime(CLOCK_MONOTONIC, tstart_cpu);
}
double cpu_timer_stop(struct timespec tstart_cpu) {
   struct timespec tstop_cpu, tresult;
   clock_gettime(CLOCK_MONOTONIC, &tstop_cpu);
   tresult.tv_sec = tstop_cpu.tv_sec - tstart_cpu.tv_sec;
   tresult.tv_nsec = tstop_cpu.tv_nsec - tstart_cpu.tv_nsec;
   double result = (double)tresult.tv_sec + (double)tresult.tv_nsec*1.0e-9;
   return(result);
}
