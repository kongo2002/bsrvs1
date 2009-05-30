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
    unsigned i;

    /* Shared-Memory-Segment einbinden und initialisieren */
    shm = (struct prim_shm *) prim_shm_attach(shmid);
    shm->primzahl=1;

    /* endlos Primzahlen produzieren ... */
    for (i = 1; ; ++i)
    {
        if (!ist_primzahl(i)) 
            continue;

        p(sid, 1);

        printf("primserv: Primzahl %u produziert!\n", i);
        shm->primzahl = i;

        v(sid, 0);
    }
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
