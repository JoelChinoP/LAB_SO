#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CLAVE_MSG 2002
#define CLAVE_SHM 2003
#define OK 0
#define ERROR -1

typedef struct
{
    long tipo;
    int fila;
    int columna;
} MENSAJE;

typedef struct
{
    char tablero[3][3];
    int turno; // 1 para X, 2 para O
} TABLERO;

void imprimir_tablero(TABLERO *t)
{
    printf("\n");
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            printf(" %c |", t->tablero[i][j] ? t->tablero[i][j] : ' ');
        }
        printf("\n-----------\n");
    }
}

int verificar_ganador(TABLERO *t, char jugador)
{
    // Verificar filas y columnas
    for (int i = 0; i < 3; i++)
    {
        if ((t->tablero[i][0] == jugador && t->tablero[i][1] == jugador && t->tablero[i][2] == jugador) ||
            (t->tablero[0][i] == jugador && t->tablero[1][i] == jugador && t->tablero[2][i] == jugador))
        {
            return 1;
        }
    }

    // Verificar diagonales
    if ((t->tablero[0][0] == jugador && t->tablero[1][1] == jugador && t->tablero[2][2] == jugador) ||
        (t->tablero[0][2] == jugador && t->tablero[1][1] == jugador && t->tablero[2][0] == jugador))
    {
        return 1;
    }

    return 0;
}

int main()
{
    int qid, shmid;
    MENSAJE msg;
    TABLERO *tablero;
    pid_t pid;

    // Crear cola de mensajes
    qid = msgget(CLAVE_MSG, IPC_CREAT | 0666);

    // Crear memoria compartida
    shmid = shmget(CLAVE_SHM, sizeof(TABLERO), IPC_CREAT | 0666);
    tablero = (TABLERO *)shmat(shmid, NULL, 0);

    // Inicializar tablero
    memset(tablero->tablero, 0, sizeof(tablero->tablero));
    tablero->turno = 1;

    pid = fork();

    if (pid == 0)
    { // Proceso hijo (jugador O)
        while (1)
        {
            if (tablero->turno == 2)
            {
                imprimir_tablero(tablero);
                printf("Jugador O - Ingrese fila (0-2) y columna (0-2): ");
                scanf("%d %d", &msg.fila, &msg.columna);

                if (tablero->tablero[msg.fila][msg.columna] == 0)
                {
                    tablero->tablero[msg.fila][msg.columna] = 'O';
                    tablero->turno = 1;

                    if (verificar_ganador(tablero, 'O'))
                    {
                        imprimir_tablero(tablero);
                        printf("¡Jugador O ha ganado!\n");
                        break;
                    }

                    msg.tipo = 1;
                    msgsnd(qid, &msg, sizeof(msg) - sizeof(long), 0);
                }
            }
            else
            {
                msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 2, 0);
            }
        }
    }
    else
    { // Proceso padre (jugador X)
        while (1)
        {
            if (tablero->turno == 1)
            {
                imprimir_tablero(tablero);
                printf("Jugador X - Ingrese fila (0-2) y columna (0-2): ");
                scanf("%d %d", &msg.fila, &msg.columna);

                if (tablero->tablero[msg.fila][msg.columna] == 0)
                {
                    tablero->tablero[msg.fila][msg.columna] = 'X';
                    tablero->turno = 2;

                    if (verificar_ganador(tablero, 'X'))
                    {
                        imprimir_tablero(tablero);
                        printf("¡Jugador X ha ganado!\n");
                        break;
                    }

                    msg.tipo = 2;
                    msgsnd(qid, &msg, sizeof(msg) - sizeof(long), 0);
                }
            }
            else
            {
                msgrcv(qid, &msg, sizeof(msg) - sizeof(long), 1, 0);
            }
        }

        // Limpiar recursos
        shmdt(tablero);
        shmctl(shmid, IPC_RMID, NULL);
        msgctl(qid, IPC_RMID, NULL);
    }

    return 0;
}