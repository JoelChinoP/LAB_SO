#include <stdio.h>
#include <pthread.h>

int c = 0;

void *funcion(void *p1)
{
    int i = 0;
    while (i < 300)
    {
        c--;
        printf("Hijo: %d\n", c);
        i++;
    }
    // Se removió pthread_exit(0)
}

int main()
{
    pthread_t hilo;
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    int error = pthread_create(&hilo, &attr, funcion, NULL);
    if (error != 0)
    {
        perror("error");
        return (-1);
    }

    int i = 0;
    while (i < 300)
    {
        c++;
        printf("Padre: %d\n", c);
        i++;
    }

    pthread_join(hilo, NULL);
    return (0);
}