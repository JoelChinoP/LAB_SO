#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
sig_atomic_t sigusr1_contador = 0;
void manejador(int nro_senal)
{
    ++sigusr1_contador;
    printf("SIGUSR1 se dio %d veces\n", sigusr1_contador);
}
int main()
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &manejador;
    sigaction(SIGUSR1, &sa, NULL);
    // por el bucle infinito el programa debe ser
    // abortado mediante SIGKILL o SIGTERM
    while (1)
        ;
    return 0;
}