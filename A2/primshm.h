#ifndef __PRIMSHM_H__
#define __PRIMSHM_H__

#include <sys/ipc.h>
#include <sys/shm.h>

struct prim_shm {
	unsigned int primzahl;
};

/* Shared-Memory-Operationen */
int prim_shm_get();
void *prim_shm_attach(int shmid);

#endif
