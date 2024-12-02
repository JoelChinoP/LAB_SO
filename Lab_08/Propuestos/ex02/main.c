#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define NUM_VEHICLES 10 // Número total de vehículos

// Semáforos
sem_t bridge;          // Semáforo que representa el acceso al puente
pthread_mutex_t mutex; // Mutex para proteger los contadores

// Contadores de vehículos por dirección
int north_count = 0;
int south_count = 0;
int east_count = 0;
int west_count = 0;

// Función para vehículos que cruzan de norte a sur
void *north_vehicle(void *arg)
{
    int id = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    if (north_count == 0 && (south_count + east_count + west_count) == 0)
    {
        sem_wait(&bridge); // Bloquea el puente
    }
    north_count++;
    pthread_mutex_unlock(&mutex);

    printf("Vehículo del Norte #%d está cruzando el puente.\n", id);
    sleep(1); // Simula el tiempo de cruce

    pthread_mutex_lock(&mutex);
    north_count--;
    if (north_count == 0)
    {
        sem_post(&bridge); // Libera el puente si ya no hay vehículos
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Función para vehículos que cruzan de sur a norte
void *south_vehicle(void *arg)
{
    int id = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    if (south_count == 0 && (north_count + east_count + west_count) == 0)
    {
        sem_wait(&bridge); // Bloquea el puente
    }
    south_count++;
    pthread_mutex_unlock(&mutex);

    printf("Vehículo del Sur #%d está cruzando el puente.\n", id);
    sleep(1); // Simula el tiempo de cruce

    pthread_mutex_lock(&mutex);
    south_count--;
    if (south_count == 0)
    {
        sem_post(&bridge); // Libera el puente si ya no hay vehículos
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Función para vehículos que cruzan de este a oeste
void *east_vehicle(void *arg)
{
    int id = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    if (east_count == 0 && (north_count + south_count + west_count) == 0)
    {
        sem_wait(&bridge); // Bloquea el puente
    }
    east_count++;
    pthread_mutex_unlock(&mutex);

    printf("Vehículo del Este #%d está cruzando el puente.\n", id);
    sleep(1); // Simula el tiempo de cruce

    pthread_mutex_lock(&mutex);
    east_count--;
    if (east_count == 0)
    {
        sem_post(&bridge); // Libera el puente si ya no hay vehículos
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

// Función para vehículos que cruzan de oeste a este
void *west_vehicle(void *arg)
{
    int id = *(int *)arg;
    free(arg);

    pthread_mutex_lock(&mutex);
    if (west_count == 0 && (north_count + south_count + east_count) == 0)
    {
        sem_wait(&bridge); // Bloquea el puente
    }
    west_count++;
    pthread_mutex_unlock(&mutex);

    printf("Vehículo del Oeste #%d está cruzando el puente.\n", id);
    sleep(1); // Simula el tiempo de cruce

    pthread_mutex_lock(&mutex);
    west_count--;
    if (west_count == 0)
    {
        sem_post(&bridge); // Libera el puente si ya no hay vehículos
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main()
{
    pthread_t threads[NUM_VEHICLES];
    sem_init(&bridge, 0, 1);          // Inicializa el puente como disponible
    pthread_mutex_init(&mutex, NULL); // Inicializa el mutex

    // Crear vehículos alternando entre direcciones
    for (int i = 0; i < NUM_VEHICLES; i++)
    {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        switch (i % 4)
        {
        case 0:
            pthread_create(&threads[i], NULL, north_vehicle, id);
            break;
        case 1:
            pthread_create(&threads[i], NULL, south_vehicle, id);
            break;
        case 2:
            pthread_create(&threads[i], NULL, east_vehicle, id);
            break;
        case 3:
            pthread_create(&threads[i], NULL, west_vehicle, id);
            break;
        }
        usleep(500000); // Introduce un retraso entre llegadas
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
