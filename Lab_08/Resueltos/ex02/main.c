#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define BUFFER_SIZE 5
// Buffer compartido
int buffer[BUFFER_SIZE];
int count = 0;
// Semáforos
sem_t empty; // Indica espacios vacíos
sem_t full;  // Indica espacios llenos
// Mutex para proteger el acceso al buffer
pthread_mutex_t mutex;
// Función del productor
void *producer(void *arg)
{
    int item, i = 0;
    while (1)
    {
        item = rand() % 100;        // Genera un número aleatorio
        sem_wait(&empty);           // Espera espacio vacío
        pthread_mutex_lock(&mutex); // Bloquea el acceso al buffer
        buffer[i % BUFFER_SIZE] = item;
        count++;
        printf("Productor produjo: %d\n", item);
        pthread_mutex_unlock(&mutex); // Desbloquea el acceso al buffer
        sem_post(&full);              // Incrementa los espacios llenos
        i++;
        sleep(1);
    }
}
// Función del consumidor
void *consumer(void *arg)
{
    int item, i = 0;
    while (1)
    {
        sem_wait(&full);            // Espera espacio lleno
        pthread_mutex_lock(&mutex); // Bloquea el acceso al buffer
        item = buffer[i % BUFFER_SIZE];
        count--;
        printf("Consumidor consumió: %d\n", item);
        pthread_mutex_unlock(&mutex); // Desbloquea el acceso al buffer
        sem_post(&empty);             // Incrementa los espacios vacíos
        i++;
        sleep(1);
    }
}
int main()
{
    pthread_t prod_thread, cons_thread;
    // Inicialización de semáforos y mutex
    sem_init(&empty, 0, BUFFER_SIZE); // Inicializa espacios vacíos
    sem_init(&full, 0, 0);            // Inicializa espacios llenos
    pthread_mutex_init(&mutex, NULL);
    // Creación de hilos
    pthread_create(&prod_thread, NULL, producer, NULL);
    pthread_create(&cons_thread, NULL, consumer, NULL);
    // Esperar a que los hilos terminen (en este caso, nunca termina)
    pthread_join(prod_thread, NULL);
    pthread_join(cons_thread, NULL);
    // Destruir semáforos y mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}