#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 2

// Buffer compartido
int buffer[BUFFER_SIZE];
int in = 0, out = 0; // Índices de entrada y salida

// Semáforos
sem_t empty; // Espacios vacíos en el buffer
sem_t full;  // Espacios llenos en el buffer

// Mutex para proteger el acceso al buffer
pthread_mutex_t mutex;

// Función del productor
void *producer(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        int item = rand() % 100;    // Genera una pieza aleatoria
        sem_wait(&empty);           // Espera un espacio vacío
        pthread_mutex_lock(&mutex); // Bloquea el buffer
        buffer[in] = item;
        printf("Productor %d produjo pieza: %d en posición %d\n", id, item, in);
        in = (in + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex); // Desbloquea el buffer
        sem_post(&full);              // Incrementa los espacios llenos
        sleep(rand() % 2 + 1);        // Simula tiempo de producción
    }
}

// Función del consumidor
void *consumer(void *arg)
{
    int id = *(int *)arg;
    while (1)
    {
        sem_wait(&full);            // Espera un espacio lleno
        pthread_mutex_lock(&mutex); // Bloquea el buffer
        int item = buffer[out];
        printf("Consumidor %d empacó pieza: %d de posición %d\n", id, item, out);
        out = (out + 1) % BUFFER_SIZE;
        pthread_mutex_unlock(&mutex); // Desbloquea el buffer
        sem_post(&empty);             // Incrementa los espacios vacíos
        sleep(rand() % 2 + 1);        // Simula tiempo de empaquetado
    }
}

int main()
{
    pthread_t producers[NUM_PRODUCERS], consumers[NUM_CONSUMERS];
    int producer_ids[NUM_PRODUCERS], consumer_ids[NUM_CONSUMERS];

    // Inicialización de semáforos y mutex
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Creación de hilos productores
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        producer_ids[i] = i + 1;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }

    // Creación de hilos consumidores
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        consumer_ids[i] = i + 1;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }

    // Esperar a que los hilos terminen (nunca ocurre en este caso)
    for (int i = 0; i < NUM_PRODUCERS; i++)
    {
        pthread_join(producers[i], NULL);
    }
    for (int i = 0; i < NUM_CONSUMERS; i++)
    {
        pthread_join(consumers[i], NULL);
    }

    // Destruir semáforos y mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
