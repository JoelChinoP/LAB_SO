#include <signal.h>
#include <unistd.h>
void trapper(int);
int main(int argc, char *argv[])
{
    int i;
    for (i = 1; i <= 64; i++)
        signal(i, trapper);
    printf("Identificativo de proceso: %d\n", getpid());
    pause();
    printf("Continuando...\n");
    return 0;
}
void trapper(int sig)
{
    signal(sig, trapper);
    printf("Recibida la señal: %d\n", sig);
}