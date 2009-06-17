#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "firstfit.h"

#define DEFAULT_NUM_AREAS	100
#define MIN_AREA_SIZE		(2*sizeof(unsigned int))
#define MAX_AREA_SIZE		100

void checked_free(void *ptr, int nr)
{
	/* Groesse des freizugebenden Speicherblocks steht drin -> ff_free benoetigt die */
	int size = *(((int *)ptr) + 1);

	/* Laufende Nummer noch heile? */
	if (*((int *)ptr) != nr) {
		fprintf(stderr, "Laufende Nummer %d kaputt!\n", nr);
		abort();
	}

	ff_free(ptr, size);
}

int main(int argc, char **argv)
{
	char **data;
	unsigned long num_areas, allocated_areas;
	int i, repetitions;

	/* Parameter vorhanden? */
	if (argc < 2) {
		/* nein -> Defaultwert */
		num_areas = DEFAULT_NUM_AREAS;
	} else {
		/* ja -> nach unsigned long konvertieren */
		char *endptr;
		num_areas = strtoul(argv[1], &endptr, 0);
		if (endptr == argv[1] || *endptr) {
			fprintf(stderr, "ungueltiger Parameter\n");
			return 1;
		}
	}
	printf("Anzahl Speicherbereiche: %lu\n", num_areas);

	/* Speicher fuer Zeigerarray besorgen */
	data = ff_alloc(num_areas * sizeof(*data));

	if (!data) {
		fprintf(stderr, "Speicher fuer Zeigerarray konnte nicht alloziert werden\n");
		return 1;
	}

	srand(time(NULL));

	/* alles 2x */
	for (repetitions = 0; repetitions < 2; ++repetitions) {	
		/* Speicherbereiche anlegen */
		for (i = 0; i < num_areas; ++i) {
			char *ptr;
			size_t size;

			size = MIN_AREA_SIZE + 
			       rand() % (MAX_AREA_SIZE - MIN_AREA_SIZE + 1);
		
			ptr = ff_alloc(size);
			if (!ptr) {
				fprintf(stderr, "Speicher alle\n");
				break;
			}

			fprintf(stderr, "Adresse: %p\n", ptr);
			data[i] = ptr;
			
			/* Laufende Nummer vorne */
			*((int *)ptr) = i;
			/* Groesse dahinter */
			*(((int *)ptr)+1) = (int) size;
		}

		allocated_areas = i;

		/* alle durch 3 teilbaren freigeben */
		for (i = 0; i < allocated_areas; i += 3) {
			checked_free(data[i], i);
		}
		/* die restlichen freigeben */
		for (i = 0; i < allocated_areas; ++i) {
			if ((i % 3) == 0) continue;
			checked_free(data[i], i);
		}
	}

	ff_free(data, num_areas * sizeof(*data));

/*
 * Diesen Code koennt ihr zum Testen des Falles "Speicher voll" einschalten
 * (indem ihr #if 0 durch #if 1 ersetzt):
 */
#if 0
	while (1) {
		void *ptr;
		if ((ptr = ff_alloc(MAX_AREA_SIZE))) {
			memset(ptr, 42, MAX_AREA_SIZE);
			continue;
		}
		fprintf(stderr, "Speicher alle\n");
		break;
	}
#endif
	return 0;
}
