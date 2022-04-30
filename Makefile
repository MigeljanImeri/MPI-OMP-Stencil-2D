CC=gcc
CFLAGS=-g -Wall -Wstrict-prototypes -std=gnu99 -fopenmp
LFLAGS=-lm -fopenmp
all:	print-2d make-2d stencil-2d print-raw stencil-2d-stack omp-stencil-2d omp-stencil-2d-stack test-mpi-read mpi-stencil-2d mpi-stencil-2d-stack
#------------------------------------------------LINKING STEPS
print-2d:	utilities.o print-2d.o
	$(CC) $(LFLAGS) -o print-2d utilities.o print-2d.o
make-2d:	utilities.o make-2d.o
	$(CC) $(LFLAGS) -o make-2d utilities.o make-2d.o
stencil-2d:	utilities.o stencil-2d.o 
	$(CC) $(LFLAGS) -o stencil-2d utilities.o stencil-2d.o 
stencil-2d-stack: utilities.o stencil-2d-stack.o 
	$(CC) $(LFLAGS) -o stencil-2d-stack utilities.o stencil-2d-stack.o 
print-raw:      utilities.o print-raw.o
	$(CC) $(LFLAGS) -o print-raw utilities.o print-raw.o
omp-stencil-2d: omp-stencil-2d.o utilities.o 
	$(CC) $(LFLAGS) -o omp-stencil-2d omp-stencil-2d.o utilities.o 
omp-stencil-2d-stack: omp-stencil-2d-stack.o utilities.o 
	$(CC) $(LFLAGS) -o omp-stencil-2d-stack omp-stencil-2d-stack.o utilities.o
test-mpi-read:	test-mpi-read.o MyMPI.o
	mpicc -lm -o test-mpi-read test-mpi-read.o MyMPI.o
mpi-stencil-2d: mpi-stencil-2d.o MyMPI.o utilities.o
	mpicc -lm -o mpi-stencil-2d mpi-stencil-2d.o MyMPI.o utilities.o 
mpi-stencil-2d-stack: mpi-stencil-2d-stack.o MyMPI.o utilities.o
	mpicc -lm -o mpi-stencil-2d-stack mpi-stencil-2d-stack.o MyMPI.o utilities.o 
#------------------------------------------------COMPILING STEPS
utilities.o: utilities.c utilities.h
	$(CC) $(CFLAGS) -c utilities.c
print-2d.o: print-2d.c utilities.h
	$(CC) $(CFLAGS) -c print-2d.c
make-2d.o: make-2d.c utilities.h
	$(CC) $(CFLAGS) -c make-2d.c
stencil-2d.o: stencil-2d.c utilities.h 
	$(CC) $(CFLAGS) -c stencil-2d.c
print-raw.o: print-raw.c utilities.h
	$(CC) $(CFLAGS) -c print-raw.c
stencil-2d-stack.o: stencil-2d-stack.c utilities.h 
	$(CC) $(CLFLAGS) -c stencil-2d-stack.c
omp-stencil-2d.o: omp-stencil-2d.c utilities.h  
	$(CC) $(CFLAGS) -c omp-stencil-2d.c
omp-stencil-2d-stack.o: omp-stencil-2d-stack.c utilities.h 
	$(CC) $(CFLAGS) -c omp-stencil-2d-stack.c
MyMPI.o:  MyMPI.c MyMPI.h
	mpicc -g -Wall -Wstrict-prototypes -std=gnu99 -c MyMPI.c	
test-mpi-read.o:  test-mpi-read.c MyMPI.h
	mpicc -g -Wall -Wstrict-prototypes -std=gnu99 -c test-mpi-read.c
mpi-stencil-2d.o: mpi-stencil-2d.c MyMPI.h
	mpicc -g -Wall -Wstrict-prototypes -std=gnu99 -c mpi-stencil-2d.c
mpi-stencil-2d-stack.o: mpi-stencil-2d-stack.c MyMPI.h
	mpicc -g -Wall -Wstrict-prototypes -std=gnu99 -c mpi-stencil-2d-stack.c

#-----------------------------------------------UTILITY FUNCTIONS
clean:
	rm -f *.o core* print-2d make-2d stencil-2d stencil-2d-stack print-raw omp-stencil-2d omp-stencil-2d-stack test-mpi-read mpi-stencil-2d mpi-stencil-2d-stack
clean-data:
	rm -f *.raw *.dat
