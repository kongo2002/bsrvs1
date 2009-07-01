#include <stdio.h>
#include <string.h>
#include "mp3.h"

int main(int argc, char **argv)
{
	int i;

	if (argc < 2)
	{
		printf("Bitte gib' mindestens eine Datei mit an: mp3 <datei1> [datei2] [...]\n");
		return 1;
	}

	for (i = 1; i < argc; i++)
	{
		idTagFile(argv[i], NULL);
	}

	return 0;
}
