#include "sync.h"

/*Erzeugt eine neue Semaphore-Menge der Groesse size*/
int erzeuge_sem(int size,int key)
{
    int semid = 0;
    /*Pruefen, ob es eine Semaphore mit dem KEY gibt*/  
    semid = semget(key,0,0);
    if (semid == -1)
    {
        /*Semaphore-Menge erzeugen*/
        if ((semid = semget(key,size,IPC_CREAT|IPC_EXCL|RIGHTS)) == -1)
        {
            perror("creating semaphore");
            return -1;
        }
        return semid;
    }
    /*-1 zurueckgeben, da die Semaphore mit key bereits existiert*/
    return -1;
}

/*Initialisiert alle Elemente in der Semaphore zu value*/
void init_sem(int semid,int size,int value)
{
    int i = 0;
    union semun cmdval;
    cmdval.val = value; 

    for (i = 0; i < size; i ++)
    {
        if (semctl(semid,i,SETVAL,cmdval) == -1)
        {
            perror("init sem");
        }
    }
}

/*Fuehrt ein P (wait) auf dem Element pos der Semaphore-Menge semid aus*/
void p(int semid,int pos)
{
    struct sembuf operation;
    operation.sem_num = pos;
    operation.sem_flg = 0; 
    operation.sem_op = -1;

    if (semop(semid,&operation,1) == -1)
    {
        perror("p");
    }
}

/*Fuehrt ein V (signal) auf dem Element pos der Semaphore-Menge semid aus*/
void v(int semid,int pos)
{
    struct sembuf operation;
    operation.sem_num = pos;
    operation.sem_flg = SEM_UNDO; 
    operation.sem_op = 1;

    if (semop(semid,&operation,1) == -1)
    {
        perror("v");
    }
}


/*Semaphore auf einen Wert setzen*/
void set_sem(int semid,int pos, int value)
{
    union semun cmdval;
    cmdval.val = value;
    if (semctl(semid,pos,SETVAL,cmdval) == -1)
    {
        perror("semctl");
    }
}

/* Wert eines Semaphors auslesen */
int get_sem(int semid,int pos)
{
    int rc = semctl(semid, pos, GETVAL);

    if (rc == -1)
        perror("semctl");

    return rc;
}
