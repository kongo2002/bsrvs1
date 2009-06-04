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

  /*HIER MUSS EUER CODE EINGEFUEGT WERDEN*/

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
	
  /*HIER MUSS EUER CODE EINGEFUEGT WERDEN*/
	
}

void kind(int pos)
{

    while (1)
    {
        /* Wagen steht an der Kreuzung */

        /* vergewissern, ob Strasse frei */
        sleep(3);

        /* claimen der entsprechenden Strassenabschnitte */
        p(semid, pos);
        p(semid, (pos+1)%4);

        /* Ueberqueren der Strasse */
        printf("Auto %d ueberquert die Strasse.\n", pos);

        /* Freigabe der entsprechenden Strassenabschnitte */
        v(semid, pos);
        v(semid, (pos+1)%4);

        /* Zurueckkehren zur Kreuzung */
        sleep(3);
    }
	
}

void programmabbruch(int sig)
{
	/*Pruefen, ob wir im Vater sind*/
	if (vaterpid == getpid())
	{	
  /*HIER MUSS EUER CODE EINGEFUEGT WERDEN*/
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
