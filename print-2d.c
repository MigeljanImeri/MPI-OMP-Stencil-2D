#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "utilities.h"


int main(int argc, char** argv) {
	
	if (argc != 2) {
		printf("usage: %s <input data file> \n",argv[0]);
		return 1;
	}	
	
	FILE* f = fopen(argv[1],"rb");

        int rows;
        int cols;

        fread(&rows, sizeof(rows), 1, f);
        fread(&cols, sizeof(cols), 1, f);

        double** matrix = malloc2D(rows,cols);

        for (int j = 0; j < rows; j++) {
                fread(matrix[j], sizeof(double), cols, f);
        }

        for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
                printf("%.4f   ",matrix[j][i]);
                }
                printf("\n");
        }

	fclose(f);

	return 0;
}
