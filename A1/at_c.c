#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void get_zombies()
{
    int zombie;

    while ((zombie = waitpid(-1, NULL, WNOHANG)) > 0)
        printf("Zombie %d gekillt.\n", zombie);

    if (zombie == -1)
    {
        if (errno != ECHILD)
        {
            perror("waitpid");
            exit(EXIT_FAILURE);
        }
    }
}


int main(int argc, char **argv)
{
    pid_t id;
    int delay = 0;
    char command[32] = "";

    while (1)
    {
        printf("Wartezeit und Befehl eingeben: ");

        if (scanf("%d, %31s", &delay, command) < 2)
        {
            fprintf(stderr, "Usage: %s <delay>, <command>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        /* zombies terminieren */
        get_zombies();

        /* job anlegen */
        id = fork();

        /* fork fehlgeschlagen */
        if (id == -1)
        {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        /* child-prozess */
        else if (id == 0)
        {
            sleep(delay);

            printf("Child: %d\n", getpid());

            /* command ausfuehren */
            if (execlp(command, command, NULL) == -1)
            {
                perror("execlp");
                exit(EXIT_FAILURE);
            }
            exit(0);
        }
        /* parent-prozess */
        else if (id > 0)
        {
            printf("Parent: %d erzeugt.\n", id);

            get_zombies();
        }
    }
    
    return 0;
}
