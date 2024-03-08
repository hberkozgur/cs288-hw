#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void radixSort(float *floatingPoint, int n, int k) {
    int pointer[16] = {0};
    float *bucket[16];
    for (int i = 0; i < 16; i++) {
        bucket[i] = (float *)malloc(n * sizeof(float));
    }

    for (int d = 0; d < k; d += 4) {
        for (int i = 0; i < n; i++) {
            unsigned int val;
            union nums {
                float f;
                unsigned int i;
            } u;
            u.f = floatingPoint[i];
            val = (u.i >> d) & 0xF;
            bucket[val][pointer[val]] = floatingPoint[i];
            pointer[val]++;
        }

        int index = 0;
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < pointer[i]; j++) {
                floatingPoint[index] = bucket[i][j];
                index++;
            }
            pointer[i] = 0;
        }
    }

    for (int i = 0; i < 16; i++) {
        free(bucket[i]);
    }
}

int main() {
    int n;
    printf("Enter the count of floating-point values: ");
    scanf("%d", &n);

    float *floatingPoint = (float *)malloc(n * sizeof(float));
    printf("Enter the floating-point values:\n");
    for (int i = 0; i < n; i++) {
        scanf("%f", &floatingPoint[i]);
    }

    radixSort(floatingPoint, n, 32);

    printf("Sorted floating-point values:\n");
    for (int i = 0; i < n; i++) {
        printf("%f\n", floatingPoint[i]);
    }

    free(floatingPoint);

    return 0;
}
