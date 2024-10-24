
#include <stdlib.h>
#include <unistd.h>
#define LEER 0
#define ESCRIBIR 1
int main(int contargs, char *args[])
{
    int descr[2]; /* Descriptores de E y S de la turbería */
    if (contargs != 3)
    {
        printf("Formato: %s comando_ent comando_sal.\n", args[0]);
        exit(1);
    }
    pipe(descr);
    if (fork() == 0)
    {
        close(descr[LEER]);
        dup2(descr[ESCRIBIR], 1);
        close(descr[ESCRIBIR]);
        execlp(args[1], args[1], NULL);
        perror(args[0]);
    }
    else
    {
        close(descr[ESCRIBIR]);
        dup2(descr[LEER], 0);
        close(descr[LEER]);
        execlp(args[2], args[2], NULL);
        perror(args[0]);
    }
}