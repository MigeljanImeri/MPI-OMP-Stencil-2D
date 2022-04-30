#ifndef UTILITIES_H_
#define UTILITIES_H_

#include <time.h>

double** malloc2D(int jmax, int imax);
void cpu_timer_start(struct timespec *tstart_cpu);
double cpu_timer_stop(struct timespec tstart_cpu);
#endif
