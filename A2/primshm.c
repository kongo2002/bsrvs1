#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "primshm.h"

/* shared memory operationen */
int prim_shm_get()
{
	int shmid = 0;
	key_t key = ftok(".", 42);
	shmid = shmget(key, 0, 0);

	if (shmid == -1) {
		shmid = shmget(key, sizeof(struct prim_shm), IPC_EXCL|IPC_CREAT|0600);
		if (shmid == -1) {
			perror("create shared memory");
			exit(1);
		}
	}

	return shmid;
}

void *prim_shm_attach(int shmid)
{
	void *pmem = NULL;

	pmem = shmat(shmid, NULL, 0);
	if (pmem == (void *) -1) {
		perror("shmat");
		exit (1);
	}

	return pmem;
}
