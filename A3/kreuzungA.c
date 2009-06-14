#include "sync.h"
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ANZAHL_AUTOS 4
int autopids[ANZAHL_AUTOS];
enum STATUS {FAHREN,HERANFAHREN,WARTEN};
void programmabbruch(int);
int erzeugeauto(int);
void kind(int);
void vater();

int vaterpid = 0;
int semid = 0;

int main()
{
    int i = 0;

    /*Signalhandler registrieren*/
    struct sigaction aktion;
    aktion.sa_handler = &programmabbruch;
    sigemptyset(&aktion.sa_mask);

    if (sigaction(SIGINT,&aktion,NULL) == -1)
    {
        perror("set actionhandler");
        exit(EXIT_FAILURE);
    }

    /*Die Kindprozesse erben den Signalhandler, da wir diesen bereits vor dem fork registrieren. 
    Damit der Vater (Kreuzung) weiss, dass er der Vater ist, speichern wir an dieser Stelle die PID.*/
    vaterpid = getpid();

    /* Semaphoren initialisieren */
    semid = erzeuge_sem(ANZAHL_AUTOS+1, 0xaffe);
    
    if (semid == -1)
    {
        fprintf(stderr, "Semaphore noch in Benutzung.\n");
        exit(-1);
    }

    init_sem(semid, ANZAHL_AUTOS+1, 1);

    for (i = 0; i < ANZAHL_AUTOS; i++)
    {
        autopids[i] = erzeugeauto(i);
        sleep(1);
    }   
    
    vater();

    return 0;
}

void vater()
{
    printf("Vater hat alle %d Autos erstellt.\n", ANZAHL_AUTOS);

    while (1)
        sleep(1);
    
}

void kind(int pos)
{
    enum STATUS state = HERANFAHREN;

    while (1)
    {
        /* Wagen steht an der Kreuzung */
        state = WARTEN;
        p(semid, pos);
        printf("Auto %d steht an der Kreuzung.\n", pos);

        /* vergewissern, ob Strasse frei */
        do
        {
            sleep(3);
            printf("Auto %d wartet auf freie Fahrt.\n", pos);
        }
        while (get_sem(semid, (pos+1)%4) == 0 || get_sem(semid, ANZAHL_AUTOS) == 0);

        /* claimen der entsprechenden Strassenabschnitte */
        p(semid, ANZAHL_AUTOS);

        /* Ueberqueren der Strasse */
        state = FAHREN;
        printf("Auto %d ueberquert die Strasse.\n", pos);
        sleep(1);

        /* Freigabe der entsprechenden Strassenabschnitte */
        v(semid, pos);
        v(semid, ANZAHL_AUTOS);
        printf("Auto %d hat die Strasse ueberquert.\n", pos);

        /* Zurueckkehren zur Kreuzung */
        state = HERANFAHREN;
        printf("Auto %d kehrt zur Kreuzung zurueck.\n", pos);
        sleep(3);
    }
}

void programmabbruch(int sig)
{
    /*Pruefen, ob wir im Vater sind*/
    if (vaterpid == getpid())
    {   
        int i;
        for (i=0; i<ANZAHL_AUTOS; ++i)
        {
            kill(autopids[i], SIGTERM);
            printf("Auto %d gekillt.\n", i);
        }

        /* TODO: waitpid() */

        semctl(semid, 0, IPC_RMID);
        printf("semaphore %d removed.\n", semid);

        printf("Programm abgebrochen.\n");
        exit(1);
    }
}

int erzeugeauto(int pos)
{
    int pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);     
    }
    else if (pid == 0)
    {
        kind(pos);
        return 0;
    }
    else
    {
        return pid;
    }
}
