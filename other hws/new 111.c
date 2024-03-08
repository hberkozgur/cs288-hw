#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>
#include <string.h>

// Function to perform counting sort on an array
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

// Function to convert decimal to binary
long long decToBin(int n) {
  long long binaryNum = 0;
  int remainder, i = 1;
  while (n != 0) {
    remainder = n % 2;
    n /= 2;
    binaryNum += remainder * i;
    i *= 10;
  }
  return binaryNum;
}

int main(int argc, char *argv[]) {
  long fileSize, value, i;
  int integers[1000] = {0}, newArr[1000] = {0}, begin, remainder, num, length;
  int *array_one, *buffer, rank, size;
  
  // Initialize MPI
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  char fileName[50];
  FILE *fp;

  if (rank == 0) { // Master process
    printf("Enter the name of the binary file to be sorted: ");
    scanf("%s", fileName);

    // Read binary file
    fp = fopen(fileName, "rb");
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    
    // Allocate memory and read data from file
    array_one = malloc(fileSize);
    fread(array_one, fileSize, 1, fp);
    fclose(fp);
  }

  // Broadcast file size to all processes
  MPI_Bcast(&fileSize, 1, MPI_LONG, 0, MPI_COMM_WORLD);

  // Allocate memory for buffer
  buffer = malloc(fileSize);

  if (rank == 0) {
    memcpy(buffer, array_one, fileSize);
    free(array_one);
  }

  // Calculate length of data and number of elements for each process
  length = fileSize / 4;
  num = length / size;
  begin = rank * num;
  remainder = begin + num;

  // Broadcast data to all processes
  MPI_Bcast(buffer, length, MPI_INT, 0, MPI_COMM_WORLD);

  // Perform counting sort on the assigned portion of data
  for (i = begin; i < remainder; i++) {
    value = buffer[i];
    integers[value] += 1;
  }

  // Reduce the counted values across all processes
  MPI_Reduce(integers, newArr, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) { // Master process
    // Write the sorted data back to the file
    fp = fopen(fileName, "wb");
    for (int x = 0; x < 1000; x++) 
      for (int z = 0; z < newArr[x]; z++) 
        fwrite(&x, sizeof(int), 1, fp);
    fclose(fp);
  }

  // Finalize MPI
  MPI_Finalize();

  return 0;
}
