#include <stdio.h>
#include <stdlib.h>
#include "utilities.h"

int main(int argc, char** argv) {

	if (argc != 5) {
		printf("usage: %s <num_rows> <num_cols> <num_iterations> <input_file>\n",argv[0]);
		return 1;
	}

	int rows = atoi(argv[1]);
	int cols = atoi(argv[2]);
	int iterations = atoi(argv[3]);
	char* input_file = argv[4];

	FILE* f = fopen(input_file,"rb");

	for (int num = 0; num < iterations; num++) {
		
		double** matrix = malloc2D(rows,cols);
		
		fread(matrix[0],sizeof(double),rows*cols,f);

		printf("Iteration: %d ----------------------\n",num);
		for (int j = 0; j < rows; j++) {
		for (int i = 0; i < cols; i++) {
			printf("%.4f  ",matrix[j][i]);
			}
			printf("\n");
		}
		free(matrix);
	}	
}
