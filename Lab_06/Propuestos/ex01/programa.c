#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct params
{
    int fila;
    float escalar;
    float matriz[3][3];
};

void mostrar_matriz(float m[3][3], char *mensaje)
{
    printf("\n%s\n", mensaje);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf("%.2f\t", m[i][j]);
        }
        printf("\n");
    }
}

void *multiplicar_fila(void *arg)
{
    struct params *p = (struct params *)arg;
    printf("Hilo multiplicando fila %d por %.2f\n", p->fila, p->escalar);

    for (int j = 0; j < 3; j++)
    {
        p->matriz[p->fila][j] *= p->escalar;
        usleep(100000); // Simular trabajo
    }
    return NULL;
}

int main()
{
    pthread_t hilos[3];
    struct params p[3];
    float escalar = 5.0;

    // Inicializar matriz
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            p[i].matriz[i][j] = rand() % 10;
        }
    }

    mostrar_matriz(p[0].matriz, "Matriz original:");

    // Crear un hilo por cada fila
    for (int i = 0; i < 3; i++)
    {
        p[i].fila = i;
        p[i].escalar = escalar;
        pthread_create(&hilos[i], NULL, multiplicar_fila, &p[i]);
    }

    // Esperar a todos los hilos
    for (int i = 0; i < 3; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    mostrar_matriz(p[0].matriz, "Matriz resultante:");
    return 0;
}