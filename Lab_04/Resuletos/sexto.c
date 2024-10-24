#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
void sighup();
void sigint();
void sigquit();
void main()
{
    int pid;

    if ((pid = fork()) < 0)
    {
        perror("fork");
        exit(1);
    }
    if (pid == 0)
    { /* hijo */
        signal(SIGHUP, sighup);
        signal(SIGINT, sigint);
        signal(SIGQUIT, sigquit);
        for (;;)
            ; /* ciclo por un error */
    }
    else
    { /* padre */
        printf("\nPAdRE: enviando SIGHUP\n\n");
        kill(pid, SIGHUP);
        sleep(3); /* pausa por 3 segundos */
        printf("\nPAdRE: enviando SIGINT\n\n");
        kill(pid, SIGINT);
        sleep(3); /* pausa por 3 segundos */
        printf("\nPAdRe: enviando SIGQUIT\n\n");
        kill(pid, SIGQUIT);
        sleep(3);
    }
}
void sighup()
{
    signal(SIGHUP, sighup); /* señal reseteada */
    printf("HIjo: yo he recibido un SIGHUP\n");
}
void sigint()
{
    signal(SIGINT, sigint); /* señal reseteada */
    printf("Hijo: Yo he recibido un SIGINT\n");
}
void sigquit()
{
    printf("Mi pApY me mato!!!\n");
    exit(0);
}