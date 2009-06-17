#include <stdio.h>
#include <stdlib.h>
#include "firstfit.h"

/* die Groesse des Speicherpools in Bytes */
#define MEM_POOL_SIZE	((unsigned long) (1024*8))      /* 8 Kilobyte */
/* 1 Bit in der Freispeicher-Bitliste repraesentiert 16 Bytes im Speicherpool */
#define CHUNK_SIZE	16

/* der Speicherpool */
static char mem_pool[MEM_POOL_SIZE];
/* die Bitliste, die anzeigt, welche Bloecke frei sind: 1 Bit fuer jew. CHUNK_SIZE Bytes */
static unsigned char free_list[MEM_POOL_SIZE/CHUNK_SIZE/8];

/* ----------------------------- Hilfsfunktionen ----------------------------- */

/* Funktion zum Debuggen, die die Freispeicher-Bitliste ausgibt */
#ifdef DEBUG
static void
dump_free_mem(void)
{
	int i;

	fprintf(stderr, "Freispeicherliste: ");
	for (i = 0; i < sizeof(free_list); ++i) {
		if ((i % 32) == 0) {
			fprintf(stderr, "\n%03X: ", i);
		}
		fprintf(stderr, "%02X ", free_list[i]);
	}
	fprintf(stderr, "\n");
}
#else
#  define dump_free_mem()
#endif

/* rechnet aus, wie viele Chunks fuer 'size' Bytes belegt werden muessen
   (rundet automatisch auf) */
static size_t
size_to_chunks(size_t size)
{
	if ((size % CHUNK_SIZE) != 0) {
		return size / CHUNK_SIZE + 1;
	} else {
		return size / CHUNK_SIZE;
	}
}

/* testet, ob im Bitfeld bitfield das Bit bitnr gesetzt ist:
 * liefert 0, wenn es nicht gesetzt ist, sonst eine Zahl > 0 */
static int
bit_is_set(unsigned char *bitfield, unsigned bitnr)
{
	return bitfield[bitnr/8] & (1 << (7 - bitnr % 8));
}

/* setzt im Bitfeld bitfield das Bit mit der Nummer bitnr */
static void
set_bit(unsigned char *bitfield, unsigned bitnr)
{
	bitfield[bitnr/8] |= 1 << (7 - bitnr % 8);
}

/* loescht im Bitfeld bitfield das Bit mit der Nummer bitnr */
static void
clear_bit(unsigned char *bitfield, unsigned bitnr)
{
	bitfield[bitnr/8] &= ~(1 << (7 - bitnr % 8));
}

/* ----------------------------- Speicherverwaltung ----------------------------- */

void *
ff_alloc(size_t size)
{
    int i, j;
    size_t chunks;

	dump_free_mem();

    /* Anzahl Chunks bestimmen */
    chunks = size_to_chunks(size);

#ifdef DEBUG
    fprintf(stderr, "%u (%u chunks) angefordert.\n", 
            (unsigned)size, 
            (unsigned)chunks);
#endif

    j = 0;

    /* freie Speicherstelle finden */
    for (i=0; i<sizeof(free_list)*8; ++i)
    {
        if (!bit_is_set(free_list, i))
            ++j;
        else
            j = 0;

        /* Platz gefunden */
        if (j == chunks)
        {
            /* Bitfeld aktualisieren */
            for (j=i-chunks+1; j<=i; ++j)
                set_bit(free_list, j);

#ifdef DEBUG
            fprintf(stderr, "Reservieren: Ab Chunk %d\n", i-(int)chunks+1);
#endif
            
            /* Position zurueckgeben */
            return (void *) (mem_pool + ((i-chunks+1)*CHUNK_SIZE));
        }
    }

	return NULL;
}

void
ff_free(void *ptr, size_t size)
{
    int i;
    size_t chunks, pos;

	dump_free_mem();

    /* Anzahl Chunks bestimmen */
    chunks = size_to_chunks(size);

#ifdef DEBUG
    fprintf(stderr, "%u (%u chunks) freizugeben.\n", 
            (unsigned)size, 
            (unsigned)chunks);
#endif

    /* Entsprechende Startposition im Bitfeld */
    pos = ((char *)ptr - mem_pool) / CHUNK_SIZE;

#ifdef DEBUG
    fprintf(stderr, "Freigeben: Ab Chunk %d\n", (int)pos);
#endif

    /* Positionen im Bitfeld freigeben */
    for (i=pos; i<pos+chunks; ++i)
        clear_bit(free_list, i);
}
