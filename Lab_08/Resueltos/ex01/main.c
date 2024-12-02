#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
// Declaración de semáforos
sem_t semA;
sem_t semB;
// Función para el Hilo A
void *threadA(void *arg)
{
    while (1)
    {
        sem_wait(&semA); // Espera hasta que el semáforo de A sea desbloqueado
        printf("Hilo A: ¡Primero!\n");
        sleep(1);        // Simula algún trabajo
        sem_post(&semB); // Desbloquea el semáforo de B
    }
    return NULL;
}
// Función para el Hilo B
void *threadB(void *arg)
{
    while (1)
    {
        sem_wait(&semB); // Espera hasta que el semáforo de B sea desbloqueado
        printf("Hilo B: ¡Después de A!\n");
        sleep(1);        // Simula algún trabajo
        sem_post(&semA); // Desbloquea el semáforo de A
    }
    return NULL;
}
int main()
{
    pthread_t tA, tB;
    // Inicialización de semáforos
    sem_init(&semA, 0, 1); // semA comienza desbloqueado (valor 1)
    sem_init(&semB, 0, 0); // semB comienza bloqueado (valor 0)
    // Creación de los hilos
    pthread_create(&tA, NULL, threadA, NULL);
    pthread_create(&tB, NULL, threadB, NULL);
    // Espera a que los hilos terminen (en este caso, nunca terminarán)
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    // Destrucción de los semáforos
    sem_destroy(&semA);
    sem_destroy(&semB);
    return 0;
}