#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <unistd.h>
#include <errno.h>

#define CLAVE_MSG 1234
#define CLAVE_MEM 5678

// Estructura del mensaje
typedef struct
{
    long tipo;
    int posicion;
} MENSAJE;

// Función para imprimir la matriz del juego
void imprimirMatriz(char *matriz)
{
    for (int i = 0; i < 9; i++)
    {
        printf("%c ", matriz[i]);
        if ((i + 1) % 3 == 0)
            printf("\n");
    }
}

// Función principal del proceso de juego
void jugar(int tipo, int msgid, char *matriz)
{
    MENSAJE mensaje;
    int posicion;
    char simbolo = (tipo == 1) ? 'X' : 'O';

    while (1)
    {
        if (tipo == 1)
        { // Jugador X
            printf("Jugador X, ingrese posición (0-8): ");
            scanf("%d", &posicion);
            mensaje.tipo = 2; // Enviar al proceso O
        }
        else
        { // Jugador O
            printf("Esperando a Jugador X...\n");
            mensaje.tipo = 1; // Enviar al proceso X
        }

        mensaje.posicion = posicion;
        matriz[posicion] = simbolo; // Actualiza matriz compartida

        // Enviar mensaje a otro proceso
        if (msgsnd(msgid, &mensaje, sizeof(int), 0) == -1)
        {
            perror("msgsnd");
            exit(EXIT_FAILURE);
        }

        imprimirMatriz(matriz); // Muestra matriz actualizada

        // Esperar mensaje del otro proceso
        if (msgrcv(msgid, &mensaje, sizeof(int), tipo, 0) == -1)
        {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }
    }
}

int main()
{
    int msgid, shmid;
    char *matriz;

    // Crear cola de mensajes
    msgid = msgget(CLAVE_MSG, IPC_CREAT | 0666);
    if (msgid == -1)
    {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Crear memoria compartida
    shmid = shmget(CLAVE_MEM, 9 * sizeof(char), IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    matriz = (char *)shmat(shmid, NULL, 0);
    if (matriz == (char *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    // Inicializar matriz con espacios en blanco
    for (int i = 0; i < 9; i++)
        matriz[i] = ' ';

    // Crear procesos para los jugadores
    if (fork() == 0)
    {
        jugar(1, msgid, matriz); // Proceso para jugador X
    }
    else
    {
        jugar(2, msgid, matriz); // Proceso para jugador O
    }

    // Liberar recursos
    shmdt(matriz);
    shmctl(shmid, IPC_RMID, NULL);
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
