#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generateRandomMatrix(int matrix[100][100], int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 100; // Generate random numbers between 0 and 99
        }
    }
}

void generateIdentityMatrix(int matrix[100][100], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            matrix[i][j] = (i == j) ? 1 : 0; // Set diagonal elements to 1, others to 0
        }
    }
}

void writeMatrixToFile(int matrix[100][100], int n, const char* filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    // Write the dimension of the matrix
    fwrite(&n, sizeof(int), 1, file);

    // Write the elements of the matrix
    for (int i = 0; i < n; i++) {
        fwrite(matrix[i], sizeof(int), n, file);
    }

    fclose(file);
}

int main() {
    int n = 5; // Change this to the desired dimension of the matrices
    int matrix1[100][100], matrix2[100][100];

    // Generate random matrix
    generateRandomMatrix(matrix1, n);

    // Generate identity matrix
    generateIdentityMatrix(matrix2, n);

    // Write matrix1 to matrix1.bin
    writeMatrixToFile(matrix1, n, "matrix1.bin");

    // Write matrix2 to matrix2.bin
    writeMatrixToFile(matrix2, n, "matrix2.bin");

    printf("Matrices generated and written to files successfully.\n");

    return 0;
}
