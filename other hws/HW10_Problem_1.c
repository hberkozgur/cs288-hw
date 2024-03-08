#include <stdio.h> #include <mpi.h>
#include <stdlib.h> #include <string.h>
void countingSort(int array[], int size) {
  int i, output[1000], max = array[1000], count[1000];
  for (i = 0; i < size; i++) {
    if (array[i] > max)
      max = array[i];
  }
  for (int i = 0; i <= max; ++i) count[i] = 0;
  for (int i = 0; i < size; i++) count[array[i]]++;
  for (int i = 1; i <= max; i++) count[i] += count[i - 1];
  for (int i = size - 1; i >= 0; i--) {
    output[count[array[i]] - 1] = array[i];
    count[array[i]]--;
  }
  for (int i = 0; i < size; i++) array[i] = output[i]; //array copied
}
long long decToBin(int n) {
  long long binaryNum = 0;
  int remainder, i = 1;
  while (n != 0) {
    remainder = n % 2;
    n /= 2;
    binaryNum += remainder * i;
    i *= 10;
  }
  return binaryNum;  }//converted bin num  
int main(int argc, char * argv[]) {
  long fileSize, value, i;
  int integers[1000] = {0}, newArr[1000] = {0}, begin, remainder, num, length;
  int * array_one, * buffer, rank, size;
  MPI_Init( & argc, & argv);
  MPI_Comm_rank(MPI_COMM_WORLD, & rank);
  MPI_Comm_size(MPI_COMM_WORLD, & size);
  char fileName[50];
  FILE * fp;
  if (rank == 0) { //if not slave
    printf("Enter name of binary file to be sorted: ");
    scanf("%s", fileName);
    fp = fopen(fileName, "rb");
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    rewind(fp);
    array_one = malloc(fileSize);
    fread(array_one, fileSize, 1, fp);
    fclose(fp);  }
  MPI_Bcast(&fileSize, 1, MPI_LONG, 0, MPI_COMM_WORLD);
  buffer = malloc(fileSize);
  if (rank == 0) {
    memcpy(buffer, array_one, fileSize);
    free(array_one);  }
  length = fileSize / 4;
  num = length / size;
  begin = rank * num;
  remainder = begin + num;
  MPI_Bcast(buffer, length, MPI_INT, 0, MPI_COMM_WORLD);
  for (i = begin; i < remainder; i++) {
    value = buffer[i];
    integers[value] += 1;  }
  MPI_Reduce(integers, newArr, 1000, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
  if (rank == 0) { //if root/master
    fp = fopen(fileName, "wb");
    for (int x = 0; x < 1000; x++) 
      for (int z = 0; z < newArr[x]; z++) 
        fwrite( & x, sizeof(int), 1, fp);  }
  MPI_Finalize();
  return 0;  }