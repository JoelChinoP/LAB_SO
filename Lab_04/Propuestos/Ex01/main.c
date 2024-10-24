#include <stdio.h>
#include <wait.h>
#include <signal.h>
#include <sys/types.h> // Necesario para pid_t y fork()
#include <unistd.h>    // Necesario para fork()

int main() {
    int stat;
    pid_t pid;
    
    if ((pid = fork()) == 0) {
        while(1);  // Proceso hijo en bucle infinito
    }
    else {
        kill(pid, SIGINT);    // Envía señal SIGINT al proceso hijo
        wait(&stat);          // Espera a que termine el proceso hijo
        
        if (WIFSIGNALED(stat)) {
            printf("El hijo terminó debido a: %s\n", 
                   strsignal(WTERMSIG(stat)));  // Usa strsignal en lugar de psignal
        }
    }
    return 0;
}