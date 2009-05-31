#include <stdio.h>

#include "primshm.h"
#include "sync.h"

int sid = 0;

/* Shared-Memory-ID */
static int shmid = 0;

/* naive Implementierung eines Primzahltests */
int ist_primzahl(unsigned x)
{
    int i;
    if (x < 2) 
        return 0;

    for (i = 2; i * i <= x; ++i)
    {
        if ((x % i) == 0)
            return 0;
    }
    return 1;
}

/* Primzahl-Fabrik */
void produziere_primzahlen()
{
    struct prim_shm *shm = NULL;
    unsigned i, j;

    /* Shared-Memory-Segment einbinden und initialisieren */
    shm = (struct prim_shm *) prim_shm_attach(shmid);
    
#ifndef BUFFER
    shm->primzahl = 1; 

    for (i = 1; ; ++i)
    {
        if (!ist_primzahl(i)) 
            continue;

        p(sid, 1);

        printf("primserv: Primzahl %u produziert!\n", i);
        shm->primzahl = i;

        v(sid, 0);
    }
#else
    for (i=0; i<BUFFER_SIZE; ++i)
        shm->primzahl[i] = 0;

    i = 0;

    while (1)
    {
        j = 0;

        p(sid, 1);

        while (j < BUFFER_SIZE)
        {
            if (!ist_primzahl(++i)) 
                continue;

            printf("primserv: Primzahl %u produziert!\n", i);
            shm->primzahl[j++] = i;
        }

        v(sid, 0);
    }
#endif
}

int main(void)
{
    /* Semaphore und Shared Memory einrichten */
    shmid = prim_shm_get();

    sid = prim_sem_get();
    prim_sem_init(sid);

    produziere_primzahlen();

    return 0;
}
