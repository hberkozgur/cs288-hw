#include <stdio.h>

void printMatrix(int matrix[100][100], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int n = 5; // Change this to the dimension of the matrices used in multiplication

    // Perform matrix multiplication and write the result to result.bin

    // Open result.bin for reading
    FILE *resultFile = fopen("result.bin", "rb");
    if (resultFile == NULL) {
        printf("Error opening result.bin\n");
        return 1;
    }

    // Read the dimension of the result matrix
    int resultDimension;
    fread(&resultDimension, sizeof(int), 1, resultFile);

    // Check if the result dimension matches the expected dimension
    if (resultDimension != n) {
        printf("Error: Result dimension does not match the expected dimension.\n");
        fclose(resultFile);
        return 1;
    }

    // Read the elements of the result matrix
    int resultMatrix[100][100];
    for (int i = 0; i < n; i++) {
        fread(resultMatrix[i], sizeof(int), n, resultFile);
    }

    // Close the result file
    fclose(resultFile);

    // Print the matrices
    printf("Matrix1:\n");
    printMatrix(matrix1, n);

    printf("Matrix2:\n");
    printMatrix(matrix2, n);

    printf("Result Matrix:\n");
    printMatrix(resultMatrix, n);

    return 0;
}

