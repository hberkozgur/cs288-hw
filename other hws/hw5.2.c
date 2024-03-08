#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void radixSort(float *floatingPoint, int n, int k) {
    int pointer[2][16] = {0};
    float *bucket[2][16];
    for (int i = 0; i < 16; i++) {
        bucket[0][i] = (float *)malloc(n * sizeof(float));
        bucket[1][i] = (float *)malloc(n * sizeof(float));
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
            if (floatingPoint[i] < 0) {
                bucket[0][val][pointer[0][val]] = floatingPoint[i];
                pointer[0][val]++;
            }
            else {
                bucket[1][val][pointer[1][val]] = floatingPoint[i];
                pointer[1][val]++;
            }
        }

        int index = 0;
        for (int i = 15; i >= 0; i--) {
            for (int j = 0; j < pointer[0][i]; j++) {
                floatingPoint[index] = bucket[0][i][j];
                index++;
            }
            pointer[0][i] = 0;
        }
        for (int i = 0; i < 16; i++) {
            for (int j = 0; j < pointer[1][i]; j++) {
                floatingPoint[index] = bucket[1][i][j];
                index++;
            }
            pointer[1][i] = 0;
        }
    }

    for (int i = 0; i < 16; i++) {
        free(bucket[0][i]);
        free(bucket[1][i]);
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
