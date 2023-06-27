#include <stdio.h>

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

    // Print the result matrix
    printf("Result Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", resultMatrix[i][j]);
        }
        printf("\n");
    }

    return 0;
}

