#include <stdio.h>
#include <signal.h>
#include <wait.h>
#include <unistd.h>    // Necesario para fork() y exit()
#include <sys/types.h> // Necesario para pid_t

int val = 10;

void handler(int sig) {
    val += 5;
}

int main() {
    pid_t pid;
    
    signal(SIGCHLD, handler);  // Registra el manejador para SIGCHLD
    
    if ((pid = fork()) == 0) {
        // Proceso hijo
        val -= 3;
        exit(0);
    }
    
    // Proceso padre
    waitpid(pid, NULL, 0);  // Espera a que termine el hijo
    printf("val = %d\n", val);
    exit(0);
}