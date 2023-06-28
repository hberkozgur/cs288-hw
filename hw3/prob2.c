#include <stdio.h>

#define MAX_SIZE 10000

int main() {
    int n1, m1, n2, m2, m3 = 0;
    int values1[MAX_SIZE], values2[MAX_SIZE], result_values[2 * MAX_SIZE];
    int column_indices1[MAX_SIZE], column_indices2[MAX_SIZE], result_column_indices[2 * MAX_SIZE];
    int row_start1[MAX_SIZE + 1], row_start2[MAX_SIZE + 1], result_row_start[MAX_SIZE + 1];

    // Open sparse_matrix1.txt and sparse_matrix2.txt for reading
    FILE *file1 = fopen("sparse_matrix1.txt", "r");
    FILE *file2 = fopen("sparse_matrix2.txt", "r");

    if (file1 == NULL || file2 == NULL) {
        printf("Failed to open input files.\n");
        return 1;
    }

    // Read the number of rows and non-zero values of the matrices
    fscanf(file1, "%d", &n1);
    fscanf(file1, "%d", &m1);

    fscanf(file2, "%d", &n2);
    fscanf(file2, "%d", &m2);

    // Read the elements of the matrices (values, column indices, row starts)
    for (int i = 0; i < m1; i++) {
        fscanf(file1, "%d", &values1[i]);
    }

    for (int i = 0; i < m1; i++) {
        fscanf(file1, "%d", &column_indices1[i]);
    }

    for (int i = 0; i <= n1; i++) {
        fscanf(file1, "%d", &row_start1[i]);
    }

    for (int i = 0; i < m2; i++) {
        fscanf(file2, "%d", &values2[i]);
    }

    for (int i = 0; i < m2; i++) {
        fscanf(file2, "%d", &column_indices2[i]);
    }

    for (int i = 0; i <= n2; i++) {
        fscanf(file2, "%d", &row_start2[i]);
    }

    // Close the input files
    fclose(file1);
    fclose(file2);

    // Perform sparse matrix addition
    int i = 0, j = 0, k = 0;

    while (i < n1 && j < n2) {
        int start1 = row_start1[i];
        int end1 = row_start1[i + 1];
        int start2 = row_start2[j];
        int end2 = row_start2[j + 1];

        if (i == j) {
            while (start1 < end1 && start2 < end2) {
                if (column_indices1[start1] < column_indices2[start2]) {
                    result_values[k] = values1[start1];
                    result_column_indices[k] = column_indices1[start1];
                    start1++;
                } else if (column_indices1[start1] > column_indices2[start2]) {
                    result_values[k] = values2[start2];
                    result_column_indices[k] = column_indices2[start2];
                    start2++;
                } else {
                    result_values[k] = values1[start1] + values2[start2];
                    result_column_indices[k] = column_indices1[start1];
                    start1++;
                    start2++;
                }
                k++;
            }

            while (start1 < end1) {
                result_values[k] = values1[start1];
                result_column_indices[k] = column_indices1[start1];
                start1++;
                k++;
            }

            while (start2 < end2) {
                result_values[k] = values2[start2];
                result_column_indices[k] = column_indices2[start2];
                start2++;
                k++;
            }

            result_row_start[i + 1] = k;
            i++;
            j++;
        } else if (i < j) {
            while (start1 < end1) {
                result_values[k] = values1[start1];
                result_column_indices[k] = column_indices1[start1];
                start1++;
                k++;
            }
            result_row_start[i + 1] = k;
            i++;
        } else {
            while (start2 < end2) {
                result_values[k] = values2[start2];
                result_column_indices[k] = column_indices2[start2];
                start2++;
                k++;
            }
            result_row_start[j + 1] = k;
            j++;
        }
    }

    while (i < n1) {
        int start1 = row_start1[i];
        int end1 = row_start1[i + 1];

        while (start1 < end1) {
            result_values[k] = values1[start1];
            result_column_indices[k] = column_indices1[start1];
            start1++;
            k++;
        }
        result_row_start[i + 1] = k;
        i++;
    }

    while (j < n2) {
        int start2 = row_start2[j];
        int end2 = row_start2[j + 1];

        while (start2 < end2) {
            result_values[k] = values2[start2];
            result_column_indices[k] = column_indices2[start2];
            start2++;
            k++;
        }
        result_row_start[j + 1] = k;
        j++;
    }

    m3 = k;

    // Open sparse_result.txt for writing
    FILE *resultFile = fopen("sparse_result.txt", "w");

    if (resultFile == NULL) {
        printf("Failed to open output file.\n");
        return 1;
    }

    // Write the number of rows and non-zero values of the result matrix
    fprintf(resultFile, "%d\n", n1);
    fprintf(resultFile, "%d\n", m3);
    fprintf(resultFile, "\nvalues: ");
    // Write the elements of the result matrix (values, column indices, row starts)
    for (int i = 0; i < m3; i++) {
        fprintf(resultFile, "%d\n", result_values[i]);
    }
    fprintf(resultFile, "\ncolumn indices: ");
    for (int i = 0; i < m3; i++) {
        fprintf(resultFile, "%d\n", result_column_indices[i]);
    }
    fprintf(resultFile, "\nrow start: ");
    for (int i = 0; i <= n1; i++) {
        fprintf(resultFile, "%d\n", result_row_start[i]);
    }

    // Close the output file
    fclose(resultFile);

    return 0;
}
