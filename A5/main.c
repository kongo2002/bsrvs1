#include <stdio.h>
#include <string.h>
#include "mp3.h"

int main(int argc, char **argv)
{
    int i, j, modify = 0;
    char buffer[31];

    if (argc < 2)
    {
        printf("Bitte gib' mindestens eine Datei mit an: mp3 <datei1> [datei2] [...]\n");
        return 1;
    }

    /* ID3-Tag modifizieren? */
    if (!strncmp("--modify", argv[1], 8))
        modify = 1;

    /* Dateien/Verzeichnisse durchlaufen */
    for (i = modify+1; i < argc; i++)
    {
        if (modify)
        {
            j = 0;
            printf("%s: Geben Sie den neuen Kommentar ein: ", argv[i]);

            while (j < 31 && ((buffer[j++] = getchar()) != '\n'));

            for (j=j-1; j<31; j++)
                buffer[j] = '\0';

            idTagFile(argv[i], buffer);
        }
        else
            idTagFile(argv[i], NULL);
    }

    return 0;
}
