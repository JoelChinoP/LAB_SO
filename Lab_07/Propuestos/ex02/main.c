#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex;
pthread_cond_t cond_var;
int buffer = 0;
int available = 0;

void* productor(void* arg) {
    for (int i = 1; i <= 10; i++) {
        pthread_mutex_lock(&mutex);

        while (available == 1) {
            pthread_cond_wait(&cond_var, &mutex);
        }

        buffer = i;
        printf("Productor: produjo %d\n", buffer);
        available = 1;

        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void* consumidor(void* arg) {
    for (int i = 0; i < 10; i++) {
        pthread_mutex_lock(&mutex);

        while (available == 0) {
            pthread_cond_wait(&cond_var, &mutex);
        }

        printf("Consumidor: consumió %d\n", buffer);
        buffer = 0;
        available = 0;

        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t hilo_productor, hilo_consumidor;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond_var, NULL);

    pthread_create(&hilo_productor, NULL, productor, NULL);
    pthread_create(&hilo_consumidor, NULL, consumidor, NULL);

    pthread_join(hilo_productor, NULL);
    pthread_join(hilo_consumidor, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_var);

    return 0;
}
