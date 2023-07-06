#include <stdio.h>

#define MAX_COUNT 100
#define N_BUCKS 16
#define N_BITS 4

void radixSort(int arr[], int count) {
	int buf[N_BUCKS][MAX_COUNT];
	int countbuf[N_BUCKS] = {0};
	int nPass = sizeof(int) * 2 / N_BITS; // Assuming 4-byte integers

	for (int pass = 0; pass < nPass; pass++) {
    	int mask = (1 << N_BITS) - 1;

    	for (int i = 0; i < count; i++) {

        	unsigned int value = (arr[i] >> (pass * N_BITS)) & mask;

        	if (arr[i] < 0) {
            	value = ~value & mask;
        	}
        	buf[value][countbuf[value]++] = arr[i];
    	}

    	int index = 0;
    	for (int bucket = 0; bucket < N_BUCKS; bucket++) {
        	for (int i = 0; i < countbuf[bucket]; i++) {
            	arr[index++] = buf[bucket][i];
        	}
        	countbuf[bucket] = 0;
    	}
	}
}


int main() {
	int count;
	int arr[MAX_COUNT];

	printf("Enter the count of integers: ");
	scanf("%d", &count);

	printf("Enter the integers:\n");
	for (int i = 0; i < count; i++) {
    	scanf("%d", &arr[i]);
	}


	int posCount = 0;
	int negCount = 0;
	int posArr[MAX_COUNT];
	int negArr[MAX_COUNT];

	for (int i = 0; i < count; i++) {
    	if (arr[i] >= 0) {
        	posArr[posCount++] = arr[i];
    	} else {
        	negArr[negCount++] = arr[i];
    	}
	}


	radixSort(posArr, posCount);
	radixSort(negArr, negCount);


	int index = 0;
	for (int i = negCount - 1; i >= 0; i--) {
    	arr[index++] = negArr[i];
	}
	for (int i = 0; i < posCount; i++) {
    	arr[index++] = posArr[i];
	}

	printf("Sorted integers:\n");
	for (int i = 0; i < count; i++) {
    	printf("%d\n", arr[i]);
	}

	return 0;
}

