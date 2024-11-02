#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

struct matriz_data
{
    float matriz[3][3];
    float escalar;
    pthread_mutex_t mutex;
};

void *multiplicar_matriz(void *arg)
{
    struct matriz_data *data = (struct matriz_data *)arg;

    pthread_mutex_lock(&data->mutex);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data->matriz[i][j] *= data->escalar;
            usleep(100000);
        }
    }
    pthread_mutex_unlock(&data->mutex);
    return NULL;
}

void *mostrar_matriz(void *arg)
{
    struct matriz_data *data = (struct matriz_data *)arg;

    for (int k = 0; k < 3; k++)
    {
        pthread_mutex_lock(&data->mutex);
        printf("\nEstado actual de la matriz (iteración %d):\n", k);
        for (int i = 0; i < 3; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                printf("%.2f\t", data->matriz[i][j]);
            }
            printf("\n");
        }
        pthread_mutex_unlock(&data->mutex);
        sleep(1);
    }
    return NULL;
}

int main()
{
    pthread_t hilo_multiplicador, hilo_mostrador;
    struct matriz_data data;

    pthread_mutex_init(&data.mutex, NULL);
    data.escalar = 2.0;

    // Inicializar matriz
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            data.matriz[i][j] = rand() % 10;
        }
    }

    // Crear hilos
    pthread_create(&hilo_mostrador, NULL, mostrar_matriz, &data);
    pthread_create(&hilo_multiplicador, NULL, multiplicar_matriz, &data);

    // Esperar hilos
    pthread_join(hilo_multiplicador, NULL);
    pthread_join(hilo_mostrador, NULL);

    pthread_mutex_destroy(&data.mutex);
    return 0;
}