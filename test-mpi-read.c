#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include "MyMPI.h"
#define dtype double
int main(int argc, char **argv) {

    if (argc != 3) {
        printf("usage: mpirun -np <p> %s <in data file> <out data file from halo array> \n", argv[0]);
        exit(1);
    }

    MPI_Init(&argc, &argv);

    dtype **A, *Adata;
    int rows, cols;

    // read in the matrix.  the matrix we're reading is is the same format, but then it allocates the matrix,
    // the matrix will have the extra rows (depending on which process it is), and it will read and place the matrix
    // where it needs to be.  so the rows and cols are the same as before, but internally, it will need extra rows
    // make this function using his read() as a starting place
    read_row_striped_matrix_halo(argv[1], (void***)&A, (void**)&Adata, MPI_DOUBLE, &rows, &cols, MPI_COMM_WORLD);

    // print out the actual matrix in memory, which includes the halos, so that we can see that it is there.
    print_row_striped_matrix_halo((void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

    exchange_row_striped_matrix_halo((void**) &Adata, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);

    print_row_striped_matrix_halo((void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);
    // then, given that A2 is a halo version of the matrix, write the data to the file
    // but the data in the file, will not have the halo information, so that it will match the
    // format that we need for the final state of the 2d heat plate.
    write_row_striped_matrix_halo(argv[2], (void**)A, MPI_DOUBLE, rows, cols, MPI_COMM_WORLD);


    MPI_Finalize();
    return 0;
}
