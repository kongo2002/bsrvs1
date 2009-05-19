#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include "sync.h"

/* Semaphor-Operationen */

#define KEY 0xaffe

/* Semaphore anlegen bzw. weiterverwenden */
int prim_sem_get()
{
    int semid = 0;

    semid = semget(KEY, 0, 0);

    if (semid < 0)
    {
        semid = semget(KEY, 2, IPC_EXCL|IPC_CREAT|0600);

        if (semid < 0)
        {
            perror("shmget");
            exit(-1);
        }
    }

    return semid;
}

/* Semaphoren mit Initialwerten versehen */
void prim_sem_init(int semid)
{
    int rc = 0;

    rc = semctl(semid, 0, SETVAL, (int)1);

    if (rc < 0)
    {
        perror("semctl SETVAL");
        exit(-1);
    }
}

/* Implementierung von P/V */
void p(int semid, int sem_num)
{
    struct sembuf sop;
    
    sop.sem_num = sem_num;
    sop.sem_op = -1;
    sop.sem_flg = 0;

    if (semop(semid, &sop, 1) == -1)
    {
        perror("p()");
        exit(-1);
    }
}

void v(int semid, int sem_num)
{
    struct sembuf sop;

    sop.sem_num = sem_num;
    sop.sem_op = 1;
    sop.sem_flg = 0;

    if (semop(semid, &sop, 1) == -1)
    {
        perror("v()");
        exit(-1);
    }
}
