#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    int delay = 0;
    char command[32] = "";

    /* ansich ueberfluessig, da execlp den aktuellen 
     * Speicher ersetzt */
    while (1)
    {
        printf("Wartezeit und Befehl eingeben: ");

        if (scanf("%d, %31s", &delay, command) < 2)
        {
            fprintf(stderr, "Usage: %s <delay>, <command>\n", argv[0]);
            exit(EXIT_FAILURE);
        }

        sleep(delay);
        
        if (execlp(command, command, NULL) == -1)
        {
            perror("execlp");
            exit(EXIT_FAILURE);
        }
    }
    
    return 0;
}
