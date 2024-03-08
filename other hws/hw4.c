#include <stdio.h>

#define MAX_COUNT 100
#define NUM_BUCKETS 16
#define NUM_BITS 4

void radixSort(int arr[], int count) {
    int buffer[NUM_BUCKETS][MAX_COUNT];
    int countBuffer[NUM_BUCKETS] = {0};
    int numPasses = sizeof(int) * 2 / NUM_BITS; // Assuming 4-byte integers

    for (int pass = 0; pass < numPasses; pass++) {
        int mask = (1 << NUM_BITS) - 1;

        for (int i = 0; i < count; i++) {
            // Use unsigned int to avoid sign extension
            unsigned int value = (arr[i] >> (pass * NUM_BITS)) & mask;
            // If the number is negative, invert the value to sort in reverse order
            if (arr[i] < 0) {
                value = ~value & mask;
            }
            buffer[value][countBuffer[value]++] = arr[i];
        }

        int index = 0;
        for (int bucket = 0; bucket < NUM_BUCKETS; bucket++) {
            for (int i = 0; i < countBuffer[bucket]; i++) {
                arr[index++] = buffer[bucket][i];
            }
            countBuffer[bucket] = 0;
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

    // Split the array into positive and negative values
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

    // Sort positive and negative arrays separately
    radixSort(posArr, posCount);
    radixSort(negArr, negCount);

    // Merge the sorted arrays back into the main array
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
