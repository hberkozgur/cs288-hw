#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

void countingSort(int array[], int size) {
  int i, output[1000], max = array[0]; // Initialize max with first element
  for (i = 1; i < size; i++) {
    if (array[i] > max)
      max = array[i]; // Find the maximum element in the array
  }

  // Create an array to store the count of each element
  int count[1000];
  for (int i = 0; i <= max; ++i) count[i] = 0;

  // Count the occurrences of each element
  for (int i = 0; i < size; i++) count[array[i]]++;

  // Update the count array to store the cumulative count
  for (int i = 1; i <= max; i++) count[i] += count[i - 1];

  // Build the output array
  for (int i = size - 1; i >= 0; i--) {
    output[count[array[i]] - 1] = array[i];
    count[array[i]]--;
  }

  // Copy the sorted output back to the original array
  for (int i = 0; i < size; i++) array[i] = output[i];
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    long fileSize, value, i;
    int integers[1000] = {0}, newArr[1000] = {0}, begin, remainder, num, length;
    int *array_one, *buffer, rank, size, totalSum = 0; // Added totalSum
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    char *fileName = argv[1]; // Use provided filename

    FILE *fp;

    if (rank == 0) {
        fp = fopen(fileName, "rb");
        if (fp == NULL) {
            fprintf(stderr, "Error opening file: %s\n", fileName);
            MPI_Finalize();
            return 1;
        }

        fseek(fp, 0, SEEK_END);
        fileSize = ftell(fp);
        rewind(fp);
        
        array_one = malloc(fileSize);
        fread(array_one, fileSize, 1, fp);
        fclose(fp);
    }

    MPI_Bcast(&fileSize, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    buffer = malloc(fileSize);

    if (rank == 0) {
        memcpy(buffer, array_one, fileSize);
        free(array_one);
    }

    length = fileSize / 4;
    num = length / size;
    begin = rank * num;
    remainder = begin + num;

    MPI_Bcast(buffer, length, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = begin; i < remainder; i++) {
        value = buffer[i];
        integers[value] += 1;
    }

    MPI_Reduce(integers, newArr, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        fp = fopen(fileName, "wb");
        for (int x = 0; x < 1000; x++) 
            for (int z = 0; z < newArr[x]; z++) 
                fwrite(&x, sizeof(int), 1, fp);
        fclose(fp);
        
        for (int x = 0; x < 1000; x++) {
            totalSum += newArr[x] * x;
        }
        printf("Sum of all integers: %d\n", totalSum);
    }

    MPI_Finalize();

    return 0;
}
