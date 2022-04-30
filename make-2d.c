#include <stdlib.h>
#include <stdio.h>
#include "utilities.h"

int main(int argc, char** argv) {

	if (argc != 4) {
		printf("usage: %s <rows> <cols> <output_file>\n",argv[0]);
		return 1;
	}	
	
	int rows = atoi(argv[1]);
	int cols = atoi(argv[2]);
	char* output_file = argv[3];
	
	double** matrix = malloc2D(rows,cols);	

	for (int j = 0; j < rows; j++) {
		matrix[j][0] = 1.0;
		matrix[j][cols - 1] = 1.0;
	}

	FILE* f = fopen(output_file,"wb");
	
	fwrite(&rows,sizeof(rows),1,f);
	fwrite(&cols,sizeof(cols),1,f);

	for (int j = 0; j < rows; j++) {
		fwrite(matrix[j], sizeof(double), cols, f);	
	}

	fclose(f);
}

