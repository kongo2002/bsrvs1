#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/types.h>
#include <signal.h>

#define ANZAHL_JOBS 10

void get_slots(pid_t *childs)
{
    int i;

    for (i=0; i<ANZAHL_JOBS; ++i)
        printf("Slot %d: %d\n", i, childs[i]);
}

void get_zombies(pid_t *childs)
{
    int i;
    int zombie;

    while ((zombie = waitpid(-1, NULL, WNOHANG)) > 0)
    {
        printf("Zombie %d gekillt.\n", zombie);

        /* reset slot of process */
        for (i=0; i<ANZAHL_JOBS; ++i)
        {
            if (childs[i] == zombie)
                childs[i] = 0;
        }
    }

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
    pid_t childs[ANZAHL_JOBS];
    int i = 0;
    int slot = 0;
    int delay = 0;
    char command[32] = "";

    /* array initialisieren */
    for (i=0; i<ANZAHL_JOBS; ++i)
        childs[i] = 0;

    while (1)
    {
        printf("Nr, Wartezeit und Befehl eingeben: ");

        if ((i = scanf("%d, %d, %31s", &slot, &delay, command)) < 2)
        {
            fprintf(stderr, "Usage: %s <slot>, <delay>, <command>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        /* zombies terminieren */
        get_zombies(childs);

        /* check for correct input */
        if ((i == 2 && delay != -1 ) || slot > 9)
        {
            fprintf(stderr, "Usage: %s <slot>, <delay>, <command>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        /* job loeschen */
        else if (i == 2)
        {
            /* job vorhanden? */
            if (childs[slot] != 0)
            {
                kill(childs[slot], 9);
                childs[slot] = 0;
            }
            else
            {
                printf("Slot %d schon frei.\n", slot);
                continue;
            }
        }
        /* job hinzufuegen */
        else if (i == 3)
        {
            if (childs[slot] != 0)
            {
                printf("Slot %d belegt.\n", slot);
                continue;
            }

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

                childs[slot] = id;

                get_zombies(childs);
                get_slots(childs);
            }
        }
    }
    
    return 0;
}
