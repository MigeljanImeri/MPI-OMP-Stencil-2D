#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include "utilities.h"
#include "MyMPI.h"
#define SWAP(xnew,xold,xtmp) {(xtmp=xnew, xnew=xold, xold=xtmp);}
#define dtype double
int main(int argc, char** argv) {

	if (argc != 4) {
		printf("mpirun -np <num processes> %s <num iterations> <input file> <output file> \n",argv[0]);
		return EXIT_FAILURE;
	}

	MPI_Init(&argc, &argv);

	int num_iterations = atoi(argv[1]);
        int rows;
        int cols;
	int rank;
	int size;

	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
	//setting up matrices
	dtype** matrixA;
	dtype* Adata;
	dtype** matrixB;
	dtype* Bdata;
	dtype** temp;
	dtype* Tdata;

	//reading data into matrices (haloed) 
	read_row_striped_matrix_halo(argv[2], (void ***)&matrixA, (void **)&Adata, MPI_DOUBLE, &rows, &cols, MPI_COMM_WORLD);		
	read_row_striped_matrix_halo(argv[2], (void ***)&matrixB, (void **)&Bdata, MPI_DOUBLE, &rows, &cols, MPI_COMM_WORLD);

	//exchanging borders with matrices 
	exchange_row_striped_matrix_halo((void**) &Adata, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);
	exchange_row_striped_matrix_halo((void**) &Bdata, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

	//check if matrix is big enough to perform stencil calculation 
	if (!(BLOCK_SIZE_HALO(rank,size,rows) >= 3)) {
		MPI_Abort(MPI_COMM_WORLD, MIN_SIZE_ERROR);
		return MIN_SIZE_ERROR;
	}

	int local_rows = BLOCK_SIZE(rank,size,rows);

	if (size == 1) {
		local_rows--;
	}

	if (0 < rank && rank < size - 1) {
		local_rows++;
	}

        struct timespec start;
        cpu_timer_start(&start);
        //runs simulation of metal plate heating
        for (int num = 0; num < num_iterations; num++) {

		//print_row_striped_matrix_halo((void**)matrixA, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);
                //perform calculations
                for (int j = 1; j < local_rows; j++) {
                for (int i = 1; i < cols - 1; i++) {
                        matrixA[j][i] = (matrixB[j-1][i-1] + matrixB[j-1][i] + matrixB[j-1][i+1] + \
                                        matrixB[j][i+1] + matrixB[j+1][i+1] + matrixB[j+1][i] + \
                                        matrixB[j+1][i-1] + matrixB[j][i-1] + matrixB[j][i])/9.0;
	
                        } 
                }
		
		exchange_row_striped_matrix_halo((void**) &Adata, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);
		//swap matrices for double buffering
                if (num < num_iterations - 1) {
			SWAP(matrixA,matrixB,temp);
			SWAP(Adata,Bdata,Tdata);
			}
	}
	double time = cpu_timer_stop(start);

	write_row_striped_matrix_halo(argv[3], (void**)matrixA, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

	double max_time;

	MPI_Reduce(&time,&max_time,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

	if (rank == 0) {
		printf("time: %f seconds | iterations: %d | rows: %d | cols: %d | process rank: %d | total process: %d\n",time,num_iterations,rows,cols,rank,size);
	}

	MPI_Finalize();
}
