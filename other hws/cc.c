#include <stdio.h>

int main() {
    int n;
    int matrix1[100][100], matrix2[100][100], result_matrix[100][100];

    // Open matrix1.bin and matrix2.bin for reading
    FILE *file1 = fopen("matrix1.bin", "rb");
    FILE *file2 = fopen("matrix2.bin", "rb");

    // Read the dimension of the matrices
    fread(&n, sizeof(int), 1, file1);
    int n2;
    fread(&n2, sizeof(int), 1, file2);

    // Check if the matrices are compatible for multiplication
    if (n != n2 || n > 100) {
        printf("Error: Matrices are not compatible or the dimension is too large.\n");
        return 1;
    }

    // Read the elements of matrix1
    for (int i = 0; i < n; i++) {
        fread(matrix1[i], sizeof(int), n, file1);
    }

    // Read the elements of matrix2
    for (int i = 0; i < n; i++) {
        fread(matrix2[i], sizeof(int), n, file2);
    }

    // Close the input files
    fclose(file1);
    fclose(file2);

    // Perform matrix multiplication
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result_matrix[i][j] = 0;
            for (int k = 0; k < n; k++) {
                result_matrix[i][j] += matrix1[i][k] * matrix2[k][j];
            }
        }
    }

    // Open result.bin for writing
    FILE *resultFile = fopen("result.bin", "wb");

    // Write the dimension of the result_matrix
    fwrite(&n, sizeof(int), 1, resultFile);

    // Write the elements of the result_matrix
    for (int i = 0; i < n; i++) {
        fwrite(result_matrix[i], sizeof(int), n, resultFile);
    }

    // Close the output file
    fclose(resultFile);

    return 0;
}

