// receiver.c
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define CLAVE_MSG 2000
#define OK 0
#define ERROR -1

typedef struct
{
    long tipo;
    int info;
} MENSAJE;

int main()
{
    int qid;
    MENSAJE msg;

    qid = msgget(CLAVE_MSG, SHM_R | SHM_W);
    if (qid == ERROR)
    {
        perror("msgget:");
        exit(errno);
    }

    printf("Leyendo el primer mensaje de la cola...\n");
    if (msgrcv(qid, &msg, sizeof(MENSAJE) - sizeof(long), 0, 0) == ERROR)
    {
        perror("msgrcv:");
        exit(errno);
    }
    printf("Mensaje recibido de tipo = %ld con info = %d\n", msg.tipo, msg.info);

    // Eliminar la cola de mensajes
    if (msgctl(qid, IPC_RMID, NULL) == ERROR)
    {
        perror("msgctl:");
        exit(errno);
    }
    printf("Cola de mensajes eliminada\n");

    exit(OK);
}