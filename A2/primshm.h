#ifndef __PRIMSHM_H__
#define __PRIMSHM_H__

#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFFER_SIZE 100

#ifndef BUFFER
struct prim_shm
{
	unsigned int primzahl;
};
#else
struct prim_shm
{
    unsigned int primzahl[BUFFER_SIZE];
};
#endif

/* Shared-Memory-Operationen */
int prim_shm_get();
void *prim_shm_attach(int shmid);

#endif

