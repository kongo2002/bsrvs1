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

    /* TODO: Semaphoren initialisieren */

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
	printf("Vater hat alle 4 Autos erstellt.\n");

    while (1)
        sleep(1);
	
}

void kind(int pos)
{
    int semid;
    enum STATUS state = HERANFAHREN;

    while (1)
    {
        /* Wagen steht an der Kreuzung */
        state = WARTEN;
        printf("Auto %d steht an der Kreuzung.\n", pos);

        /* vergewissern, ob Strasse frei */
        sleep(3);
        printf("Auto %d hat geschaut, ob rechts frei ist.\n", pos);

        /* claimen der entsprechenden Strassenabschnitte */
        p(semid, pos);
        p(semid, (pos+1)%4);

        /* Ueberqueren der Strasse */
        state = FAHREN;
        printf("Auto %d ueberquert die Strasse.\n", pos);

        /* Freigabe der entsprechenden Strassenabschnitte */
        v(semid, pos);
        v(semid, (pos+1)%4);

        /* Zurueckkehren zur Kreuzung */
        state = HERANFAHREN;
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
            kill(9, autopids[i]);
            printf("Auto %d gekillt.\n", i);
        }

        /* TODO: Zombies entfernen
         * waitpid() */

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
