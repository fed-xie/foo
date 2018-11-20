void rotate(int** matrix, int matrixRowSize, int *matrixColSizes)
{
	/* rotate i like
	   01234 0
	         1
	   4     2
	   3     3
	   2     4
	   1
	   0 43210
	*/
    int offset = 0;
	int limit = matrixRowSize - 1;
	int max = limit;
	while (offset < limit) {
		for (int i=offset; i<limit; ++i) {
			int tmp = matrix[offset][i     ];
			matrix[offset][i     ] = matrix[max -i][offset]; //top
			matrix[max -i][offset] = matrix[limit ][max -i]; //left
			matrix[limit ][max -i] = matrix[i     ][limit ]; //bottom
			matrix[i     ][limit ] = tmp; //right
		}
		++offset;
		--limit;
	}
	*matrixColSizes = matrixRowSize;
}

#include <stdio.h>
int main(int argc, char** argv)
{
	int r0[] = { 5, 1, 9,11};
	int r1[] = { 2, 4, 8,10};
	int r2[] = {13, 3, 6, 7};
	int r3[] = {15,14,12,16};
	int* img[] = {r0,r1,r2,r3};
	int row = sizeof(img)/sizeof(*img);
	int col = row;
	printf("Input:\n");
	for (int i=0; i<row; ++i) {
		for (int j=0; j<col; ++j) {
			printf("%2d,", img[i][j]);
		}
		printf("\n");
	}
	rotate(img, row, &col);
	printf("\nOutput:\n");
	for (int i=0; i<row; ++i) {
		for (int j=0; j<col; ++j) {
			printf("%2d,", img[i][j]);
		}
		printf("\n");
	}
	return 0;
}