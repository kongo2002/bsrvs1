#include <stdio.h>
#include <string.h>
#include "mp3.h"

int main(int argc, char **argv)
{
    int i, j, modify = 0;
    char buffer[31];
    char tmp;

    if (argc < 2)
    {
        printf("Bitte gib' mindestens eine Datei mit an: mp3 <datei1> [datei2] [...]\n");
        return 1;
    }

    /* Kommentar modifizieren? */
    if (!strncmp("--modify", argv[1], 8))
        modify = 1;

    /* Dateien/Verzeichnisse durchlaufen */
    for (i = modify+1; i < argc; i++)
    {
        if (modify)
        {
            j = 0;
            while (((tmp = getchar()) != '\n') && j < 31)
            {
                if (tmp != '\n')
                    buffer[j++] = tmp;
            }

            buffer[j] = '\0';
            printf("buffer = %s\n", buffer);
        }
        idTagFile(argv[i], NULL);
    }

    return 0;
}
