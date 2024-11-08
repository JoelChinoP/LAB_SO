#include <stdio.h>
#include <pthread.h>

#define NUM_THREADS 4
#define NUM_ELEMENTS 100

pthread_mutex_t mutex;
int sum = 0;
int array[NUM_ELEMENTS];

void* sumArray(void* arg) {
    int thread_id = *(int*)arg;
    int local_sum = 0;

    for (int i = thread_id; i < NUM_ELEMENTS; i += NUM_THREADS) {
        local_sum += array[i];
    }

    // Sección crítica
    pthread_mutex_lock(&mutex);
    sum += local_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    // Inicializar el array
    for (int i = 0; i < NUM_ELEMENTS; i++) {
        array[i] = i + 1;
    }

    // Crear los hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, sumArray, &thread_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Suma total: %d\n", sum);

    pthread_mutex_destroy(&mutex);

    return 0;
}
