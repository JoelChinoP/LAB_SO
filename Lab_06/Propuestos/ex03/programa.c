#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_ELEMENTOS 5000
#define NUM_HILOS 4

struct rango
{
    int inicio;
    int fin;
    int *numeros;
    long suma_parcial;
};

void *sumar_rango(void *arg)
{
    struct rango *r = (struct rango *)arg;
    r->suma_parcial = 0;

    for (int i = r->inicio; i < r->fin; i++)
    {
        r->suma_parcial += r->numeros[i];
    }
    return NULL;
}

long suma_secuencial(int *numeros, int n)
{
    long suma = 0;
    for (int i = 0; i < n; i++)
    {
        suma += numeros[i];
    }
    return suma;
}

int main()
{
    int numeros[NUM_ELEMENTOS];
    pthread_t hilos[NUM_HILOS];
    struct rango rangos[NUM_HILOS];
    struct timespec inicio, fin;
    double tiempo_paralelo, tiempo_secuencial;

    // Inicializar números aleatorios
    srand(time(NULL));
    for (int i = 0; i < NUM_ELEMENTOS; i++)
    {
        numeros[i] = rand() % 100;
    }

    // Versión paralela
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    int elementos_por_hilo = NUM_ELEMENTOS / NUM_HILOS;
    for (int i = 0; i < NUM_HILOS; i++)
    {
        rangos[i].inicio = i * elementos_por_hilo;
        rangos[i].fin = (i == NUM_HILOS - 1) ? NUM_ELEMENTOS : (i + 1) * elementos_por_hilo;
        rangos[i].numeros = numeros;
        pthread_create(&hilos[i], NULL, sumar_rango, &rangos[i]);
    }

    long suma_total = 0;
    for (int i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
        suma_total += rangos[i].suma_parcial;
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);
    tiempo_paralelo = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    // Versión secuencial
    clock_gettime(CLOCK_MONOTONIC, &inicio);
    long suma_sec = suma_secuencial(numeros, NUM_ELEMENTOS);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    tiempo_secuencial = (fin.tv_sec - inicio.tv_sec) + (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    printf("Suma paralela: %ld (tiempo: %.6f segundos)\n", suma_total, tiempo_paralelo);
    printf("Suma secuencial: %ld (tiempo: %.6f segundos)\n", suma_sec, tiempo_secuencial);
    printf("Speedup: %.2fx\n", tiempo_secuencial / tiempo_paralelo);

    return 0;
}