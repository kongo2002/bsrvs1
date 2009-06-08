#include "sync.h"
#include <time.h>
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
int deadlock_observer();

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

    /* Zufallsgenerator initialisieren */
    srand(time(NULL));

    /*Die Kindprozesse erben den Signalhandler, da wir diesen bereits vor dem fork registrieren. 
    Damit der Vater (Kreuzung) weiss, dass er der Vater ist, speichern wir an dieser Stelle die PID.*/
    vaterpid = getpid();

    /* Semaphoren erzeugen */
    semid = erzeuge_sem(ANZAHL_AUTOS+2, 0xaffe);
    
    if (semid == -1)
    {
        fprintf(stderr, "Semaphore noch in Benutzung.\n");
        exit(-1);
    }

    /* Semaphoren initialisieren */
    init_sem(semid, ANZAHL_AUTOS+1, 1);

    /* Semaphore fuer Deadlock Entsperrung initialisieren */
    set_sem(semid, ANZAHL_AUTOS+1, ANZAHL_AUTOS);

    /* Autos forken */
    for (i = 0; i < ANZAHL_AUTOS; i++)
    {
        autopids[i] = erzeugeauto(i);
        sleep(1);
    }   
    
    /* Ueberwachungs-Prozess (Vater) */
    vater();

    return 0;
}

void vater()
{
    while (1)
    {
        sleep(20);

        if (deadlock_observer())
        {
            /* Zufaellige Freigabe eines der Autos */
            int freigabe = rand()%ANZAHL_AUTOS;

            printf("DEADLOCK - Freigabe fuer Auto %d\n", freigabe);
            set_sem(semid, ANZAHL_AUTOS+1, freigabe); 
        }
    }
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

        /* Vergewissern, ob Strasse frei */
        do
        {
            sleep(3);
            printf("Auto %d wartet auf freie Fahrt.\n", pos);

            /* evtl Deadlock Entsperrung */
            if (get_sem(semid, ANZAHL_AUTOS+1) == pos)
                break;
        }
        while (get_sem(semid, (pos+1)%4) == 0 || get_sem(semid, ANZAHL_AUTOS) == 0);

        /* claimen der entsprechenden Strassenabschnitte */
        p(semid, ANZAHL_AUTOS);

        /* Ueberqueren der Strasse */
        state = FAHREN;
        printf("Auto %d ueberquert die Strasse.\n", pos);
        sleep(1);

        /* Zuruecksetzen der Deadlock Sperre, falls noetig */
        if (get_sem(semid, ANZAHL_AUTOS+1) == pos)
            set_sem(semid, ANZAHL_AUTOS+1, ANZAHL_AUTOS);

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
        /* Beenden der Child-Prozesse */
        int i;
        for (i=0; i<ANZAHL_AUTOS; ++i)
        {
            kill(autopids[i], SIGTERM);
            printf("Auto %d gekillt.\n", i);
        }

        /* Entfernen der Zombies */
        while ((i = waitpid(-1, NULL, WNOHANG)) > 0)
            printf("Zombie %d entfernt.\n", i);

        /* Semaphore freigeben */
        semctl(semid, 0, IPC_RMID);
        printf("Semaphore %d entfernt.\n", semid);

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

int deadlock_observer()
{
    int i;

    /* warten alle Autos? */
    for (i=0; i<ANZAHL_AUTOS; ++i)
    {
        if (get_sem(semid, i))
            return 0;
    }

    /* ist kein Auto auf der Kreuzung? */
    if (!get_sem(semid, ANZAHL_AUTOS))
        return 0;

    return 1;
}
