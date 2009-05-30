#include <stdio.h>
#include <unistd.h>

#include "primshm.h"
#include "sync.h"

int main()
{
    unsigned j;

    /* Semaphore und Shared Memory einrichten */
    int shmid = prim_shm_get();

    int sid = prim_sem_get();

    struct prim_shm *shm = NULL;
    shm = (struct prim_shm *) prim_shm_attach(shmid);

    /* endlos Primzahlen konsumieren ... */
    while (1)
    {
        p(sid, 0);

#ifndef BUFFER
        printf("primeat: Primzahl %u konsumiert!\n", shm->primzahl);
#else
        for (j=0; j<BUFFER_SIZE; ++j)
            printf("primeat: Primzahl %u konsumiert!\n", shm->primzahl[j]);
#endif

        v(sid, 1);

        /* ein bisschen schlafenlegen ... Primzahlen Konsumieren macht muede! */
        sleep(1);
    }

    return 0;
}


