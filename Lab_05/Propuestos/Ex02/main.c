#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CLAVE_MSG 2001
#define OK 0
#define ERROR -1

typedef struct
{
    long tipo;
    char texto[50];
} MENSAJE;

int main()
{
    int qid;
    MENSAJE msg;
    pid_t pid;

    // Crear cola de mensajes
    qid = msgget(CLAVE_MSG, IPC_CREAT | IPC_EXCL | 0666);

    pid = fork();

    if (pid == 0)
    {             // Proceso hijo (receptor)
        sleep(2); // Espera 2 segundos antes de intentar leer

        printf("Hijo: Intentando leer mensaje con IPC_NOWAIT...\n");

        // Intenta leer con IPC_NOWAIT
        if (msgrcv(qid, &msg, sizeof(msg.texto), 0, IPC_NOWAIT) == ERROR)
        {
            if (errno == ENOMSG)
            {
                printf("Hijo: No hay mensajes disponibles (ENOMSG)\n");
            }
            else
            {
                perror("msgrcv");
            }
        }
        else
        {
            printf("Hijo: Mensaje recibido: %s\n", msg.texto);
        }
    }
    else
    {             // Proceso padre (emisor)
        sleep(4); // Espera 4 segundos antes de enviar

        msg.tipo = 1;
        sprintf(msg.texto, "Mensaje de prueba");

        printf("Padre: Enviando mensaje...\n");
        if (msgsnd(qid, &msg, sizeof(msg.texto), 0) == ERROR)
        {
            perror("msgsnd");
            exit(1);
        }

        printf("Padre: Mensaje enviado\n");
        sleep(1);

        // Eliminar cola de mensajes
        msgctl(qid, IPC_RMID, NULL);
    }

    return 0;
}