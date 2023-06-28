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
    int n;
    int matrix1[100][100], matrix2[100][100], resultMatrix[100][100];

    // Open matrix1.bin for reading
    FILE *matrix1File = fopen("matrix1.bin", "rb");
    if (matrix1File == NULL) {
        printf("Error opening matrix1.bin\n");
        return 1;
    }

    // Read the dimension of matrix1
    fread(&n, sizeof(int), 1, matrix1File);

    // Check if the dimension is valid
    if (n > 100) {
        printf("Error: Invalid dimension in matrix1.bin\n");
        fclose(matrix1File);
        return 1;
    }

    // Read the elements of matrix1
    for (int i = 0; i < n; i++) {
        fread(matrix1[i], sizeof(int), n, matrix1File);
    }

    // Close matrix1.bin
    fclose(matrix1File);

    // Open matrix2.bin for reading
    FILE *matrix2File = fopen("matrix2.bin", "rb");
    if (matrix2File == NULL) {
        printf("Error opening matrix2.bin\n");
        return 1;
    }

    // Read the dimension of matrix2
    int matrix2Dimension;
    fread(&matrix2Dimension, sizeof(int), 1, matrix2File);

    // Check if the dimension is valid and matches matrix1
    if (matrix2Dimension != n || matrix2Dimension > 100) {
        printf("Error: Invalid dimension in matrix2.bin\n");
        fclose(matrix2File);
        return 1;
    }

    // Read the elements of matrix2
    for (int i = 0; i < n; i++) {
        fread(matrix2[i], sizeof(int), n, matrix2File);
    }

    // Close matrix2.bin
    fclose(matrix2File);

    // Perform matrix multiplication and store the result in resultMatrix
    // ...

    // Print the matrices
    printf("Matrix1:\n");
    printMatrix(matrix1, n);

    printf("Matrix2:\n");
    printMatrix(matrix2, n);

    printf("Result Matrix:\n");
    printMatrix(resultMatrix, n);

    return 0;
}

