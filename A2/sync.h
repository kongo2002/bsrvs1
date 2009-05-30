#ifndef __SYNC_H__
#define __SYNC_H__

/* Semaphor-Operationen */
int prim_sem_get();
void prim_sem_init(int semid);

void p(int semid, int sem_num);
void v(int semid, int sem_num);

#endif

