#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int delay = 0;

    while (1)
    {
        printf("Wartezeit eingeben: ");

        if (scanf("%d", &delay) < 1)
        {
            fprintf(stderr, "Unzulaessige Eingabe.\n");
            break;
        }

        sleep(delay);

        printf("Beep.\n");
    }
    
    return 0;
}
