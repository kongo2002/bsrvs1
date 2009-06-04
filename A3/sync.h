#ifndef __SYNC_H_
#define __SYNC_H_
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>

#define RIGHTS 0600

/*Datenstruktur zum Abfragen/Setzen der Semaphoren*/
union semun {
	int              val;    /* Value for SETVAL */
	struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;  /* Array for GETALL, SETALL */
	struct seminfo  *__buf;  /* Buffer for IPC_INFO
				    (Linux-specific) */
};

/*Erzeugt eine neue Semaphore-Menge der Groesse size*/
int erzeuge_sem(int size,int key);
/*Initialisiert alle Elemente in der Semaphore zu value*/
void init_sem(int semid,int size,int value);
/*Fuehrt ein P (wait) auf dem Element pos der Semaphore-Menge semid aus*/
void p(int semid,int pos);
/*Fuehrt ein V (signal) auf dem Element pos der Semaphore-Menge semid aus*/
void v(int semid,int pos);
/*Semaphore auf einen Wert setzen*/
void set_sem(int semid,int pos, int value);
#endif
