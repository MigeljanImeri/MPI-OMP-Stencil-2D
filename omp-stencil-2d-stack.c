#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"
#include "omp.h"
#define SWAP(xnew,xold,xtmp) (xtmp=xnew, xnew=xold, xold=xtmp)

int main (int argc, char** argv) {
	
	if (argc != 5) {
		printf("usage: %s <num-iterations> <input file> <all-iterations> <num-threads>\n",argv[0]);
		return 1;
	}

	int num_iterations = atoi(argv[1]);
	char* input_file = argv[2];
	char* all_iterations = argv[3];
	int num_threads = atoi(argv[4]);

	FILE* Finput_file  = fopen(input_file,"rb");
	FILE* Fall_iterations = fopen(all_iterations,"wb");

        int rows;
        int cols;

	//get # of rows and cols from metadata of input file
        fread(&rows, sizeof(rows), 1, Finput_file);
        fread(&cols, sizeof(cols), 1, Finput_file);

        double** matrixA = malloc2D(rows,cols);
	double** temp;
	//creates matrix for double buffering
	double** matrixB = malloc2D(rows,cols);

	//reads in matrix from input file
        for (int j = 0; j < rows; j++) {
                fread(matrixA[j], sizeof(double), cols, Finput_file);
        }

	//reset file to correct position, skipping metadata
	fseek(Finput_file, 8, SEEK_SET);
	
	//reads in matrix from input file
	for (int j = 0; j < rows; j++) {
		fread(matrixB[j], sizeof(double), cols, Finput_file);
	}

	struct timespec start;
        cpu_timer_start(&start);
	//runs simulation of metal plate heating
	for (int num = 0; num < num_iterations; num++) {

		//writes plate to image stack, happens before any calculations
		for (int j = 0; j < rows; j++) {
         	       fwrite(matrixB[j], sizeof(double), cols, Fall_iterations);
        	}
	
		//perform calculations
		#pragma omp parallel for num_threads(num_threads) 
		for (int j = 1; j < rows - 1; j++) {
		for (int i = 1; i < cols - 1; i++) {
			matrixA[j][i] = (matrixB[j-1][i-1] + matrixB[j-1][i] + matrixB[j-1][i+1] + \
				       	matrixB[j][i+1] + matrixB[j+1][i+1] + matrixB[j+1][i] + \
				       	matrixB[j+1][i-1] + matrixB[j][i-1] + matrixB[j][i])/9.0;
			}
		}	

		//swap matrices for double buffering
		SWAP(matrixA,matrixB,temp);
	}
	double time = cpu_timer_stop(start);
	

	//write final plate to output file
	for (int j = 0; j < rows; j++) {
		fwrite(matrixB[j], sizeof(double), cols, Fall_iterations);
	}

	printf("time: %f seconds | iterations: %d | rows: %d | cols: %d | num_threads: %d\n",time,num_iterations,rows,cols,num_threads);

	//close files	
	fclose(Finput_file);
	fclose(Fall_iterations);

	return 0;
}
