#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_VEHICLES 10 // Número total de vehículos
// Semáforos
sem_t bridge;          // Semáforo que representa el acceso al puente
pthread_mutex_t mutex; // Mutex para proteger los contadores
int north_count = 0;   // Contador de vehículos del norte cruzando
int south_count = 0;   // Contador de vehículos del sur cruzando
// Función para simular un vehículo que cruza desde el norte
void *north_vehicle(void *arg)
{
    int id = *(int *)arg;
    free(arg);
    // Solicitar acceso al puente
    pthread_mutex_lock(&mutex);
    if (north_count == 0 && south_count == 0)
    {
        sem_wait(&bridge); // Bloquea el puente
    }
    north_count++;
    pthread_mutex_unlock(&mutex);
    // Cruza el puente
    printf("Vehículo del Norte #%d está cruzando el puente.\n", id);
    sleep(1); // Simula el tiempo que tarda en cruzar el puente
    // Sale del puente
    pthread_mutex_lock(&mutex);
    north_count--;
    if (north_count == 0)
    {
        sem_post(&bridge); // Libera el puente
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
// Función para simular un vehículo que cruza desde el sur
void *south_vehicle(void *arg)
{
    int id = *(int *)arg;
    free(arg);
    // Solicitar acceso al puente
    pthread_mutex_lock(&mutex);
    if (south_count == 0 && north_count == 0)
    {
        sem_wait(&bridge); // Bloquea el puente
    }
    south_count++;
    pthread_mutex_unlock(&mutex);
    // Cruza el puente
    printf("Vehículo del Sur #%d está cruzando el puente.\n", id);
    sleep(1); // Simula el tiempo que tarda en cruzar el puente
    // Sale del puente
    pthread_mutex_lock(&mutex);
    south_count--;
    if (south_count == 0)
    {
        sem_post(&bridge); // Libera el puente
    }
    pthread_mutex_unlock(&mutex);
    return NULL;
}
int main()
{
    pthread_t threads[NUM_VEHICLES];
    sem_init(&bridge, 0, 1);          // Inicializa el puente como disponible
    pthread_mutex_init(&mutex, NULL); // Inicializa el mutex para protección
    // Crear vehículos alternando entre norte y sur
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        if (i % 2 == 0)
        {
            pthread_create(&threads[i], NULL, north_vehicle, id);
        }
        else
        {
            pthread_create(&threads[i], NULL, south_vehicle, id);
        }
        usleep(500000); // Introducir un retraso entre llegadas
    }
    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // Destruir semáforos y mutex
    sem_destroy(&bridge);
    pthread_mutex_destroy(&mutex);
    return 0;
}