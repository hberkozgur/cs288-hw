#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    int start;
    int end;
    double sum;
} thread_data_t;

void *calculate_sum(void *arg) {
    thread_data_t *data = (thread_data_t *) arg;
    double sum = 0.0;
    for (int i = data->start; i < data->end; i++) {
        sum += 4.0 * (i % 2 == 0 ? 1 : -1) / (2.0 * i + 1);
    }
    data->sum = sum;
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <num_threads> <num_terms>\n", argv[0]);
        exit(1);
    }

    int num_threads = atoi(argv[1]);
    int num_terms = atoi(argv[2]);

    pthread_t threads[num_threads];
    thread_data_t thread_data[num_threads];

    int terms_per_thread = num_terms / num_threads;
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].start = i * terms_per_thread;
        thread_data[i].end = (i == num_threads - 1) ? num_terms : (i + 1) * terms_per_thread;
        pthread_create(&threads[i], NULL, calculate_sum, &thread_data[i]);
    }

    double pi_approximation = 0.0;
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
        pi_approximation += thread_data[i].sum;
    }

    printf("Approximation of pi: %.15f\n", pi_approximation);

    return 0;
}