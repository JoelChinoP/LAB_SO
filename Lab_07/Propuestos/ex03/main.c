#include <stdio.h>
#include <pthread.h>

#define N 3
#define NUM_THREADS 12

int A[N][N], B[N][N], C[N][N];
pthread_mutex_t mutex;

void* multiply(void* arg) {
    int thread_id = *(int*)arg;
    int row = thread_id / N;
    int col = thread_id % N;

    int sum = 0;
    for (int k = 0; k < N; k++) {
        sum += A[row][k] * B[k][col];
    }

    pthread_mutex_lock(&mutex);
    C[row][col] = sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    pthread_mutex_init(&mutex, NULL);

    // Inicializar matrices
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = i + 1;
            B[i][j] = j + 1;
        }
    }

    // Crear hilos
    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, multiply, &thread_ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Imprimir matriz resultado
    printf("Matriz C (resultado):\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
