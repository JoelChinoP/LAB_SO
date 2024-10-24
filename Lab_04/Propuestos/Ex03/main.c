#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

pid_t pid_h1, pid_h2, pid_h3;

// Manejador de señales para H2
void manejador_sigusr1(int sig) {
    printf("H2 (PID %d) recibió señal SIGUSR1 de H1\n", getpid());
}

int main() {
    pid_h1 = fork();  // Crear H1

    if (pid_h1 == 0) {
        // Código de H1
        printf("H1 (PID %d, padre PID %d)\n", getpid(), getppid());

        pid_h3 = fork();  // Crear H3
        if (pid_h3 == 0) {
            // Código de H3
            execl("/bin/bash", "bash", "-c", "echo 'Bienvenido al sistema'; date", NULL);
            exit(0);
        }

        while (1) {
            // Enviar SIGUSR1 a H2 cada 20 segundos
            sleep(20);
            kill(pid_h2, SIGUSR1);
        }
    } else {
        pid_h2 = fork();  // Crear H2

        if (pid_h2 == 0) {
            // Código de H2
            signal(SIGUSR1, manejador_sigusr1);  // Registrar manejador de SIGUSR1
            while (1) {
                pause();  // Esperar señal
            }
        } else {
            // Código del proceso padre P
            wait(NULL);  // Esperar a que terminen los hijos
            wait(NULL);
            wait(NULL);
            printf("Proceso padre (PID %d) termina.\n", getpid());
        }
    }

    return 0;
}
