#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MIN_VALUE 0
#define MAX_VALUE 999
#define COUNTER_SIZE (MAX_VALUE - MIN_VALUE + 1)

int main(int argc, char *argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char filename[256];
    if (rank == 0) {
        printf("Enter the file pathname: ");
        scanf("%s", filename);
    }
    MPI_Bcast(filename, 256, MPI_CHAR, 0, MPI_COMM_WORLD);

    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    fseek(fp, 0, SEEK_END);
    long file_size = ftell(fp);
    int num_values = file_size / sizeof(int);
    int local_num_values = num_values / size;
    if (rank == size - 1) {
        local_num_values += num_values % size;
    }

    int *local_values = malloc(local_num_values * sizeof(int));
    fseek(fp, rank * (num_values / size) * sizeof(int), SEEK_SET);
    fread(local_values, sizeof(int), local_num_values, fp);
    fclose(fp);

    int local_counter[COUNTER_SIZE] = {0};
    for (int i = 0; i < local_num_values; i++) {
        local_counter[local_values[i] - MIN_VALUE]++;
    }

    int global_counter[COUNTER_SIZE];
    MPI_Allreduce(local_counter, global_counter, COUNTER_SIZE, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    int offset = 0;
    for (int i = 0; i < rank; i++) {
        offset += num_values / size;
        if (i == size - 1) {
            offset += num_values % size;
        }
    }
    for (int i = 0; i < COUNTER_SIZE; i++) {
        for (int j = 0; j < global_counter[i]; j++) {
            local_values[offset++] = i + MIN_VALUE;
        }
    }

    if (rank == 0) {
        fp = fopen(filename, "wb");
        if (fp == NULL) {
            fprintf(stderr, "Error opening file %s\n", filename);
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        fwrite(local_values, sizeof(int), num_values, fp);
        fclose(fp);
    }

    free(local_values);
    MPI_Finalize();
}