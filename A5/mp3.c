#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "mp3.h"

#define ID3_SIZE 128

void idTagFile(const char *fileName,char *comment)
{
    FILE *datei = NULL;
    struct stat finfo = {0};
    int rc = 0;

    /* nicht aktuelles oder vorheriges Verzeichnis */
	if (!strncmp(".", fileName, 2) || !strncmp("..", fileName, 3))
        return;

    /* Datei-Eigenschaften auslesen */
    if ((rc = stat(fileName, &finfo)) == -1)
    {
        fprintf(stderr, "Fehler beim Auslesen der Datei: %s\n", fileName);
        return;
    }
	
    /* Pruefen, ob regulaere Datei */
    if (!S_ISREG(finfo.st_mode))
    {
        fprintf(stderr, "Keine regulaere Datei: %s\n", fileName);
        return;
    }

    /* Pruefen, ob mp3-Datei vorliegend */
	if (strncmp(fileName + strlen(fileName) - 4, ".mp3", 4))
    {
		printf("%s: ausgelassen\n", fileName);
		return;
	}

    /* Datei oeffnen */
    if ((datei = fopen(fileName, "r")) == NULL)
    {
        fprintf(stderr, "Fehler beim Oeffnen der Datei: %s\n", fileName);
        return;
    }

    /* An die letzten 128 Bytes springen */
    if (fseek(datei, -ID3_SIZE, SEEK_END) == -1)
    {
        fprintf(stderr, "Fehler beim Auslesen des ID3-Tags: %s\n", fileName);
        fclose(datei);
        return;
    }
}

void idTagDir(const char *dirName, char *comment)
{
	/* HIER MUESST IHR EUREN CODE EINFUEGEN */

	if (!strncmp(".", dirName, 2) || !strncmp("..", dirName, 3))
               	return;
	
	/* HIER MUESST IHR EUREN CODE EINFUEGEN */

	printf("%s: Betrete Verzeichnis ...\n", dirName);

	/* HIER MUESST IHR EUREN CODE EINFUEGEN */

	printf("%s: Verlasse Verzeichnis ...\n", dirName);
}

/* Die Informationen im Puffer, auf den buffer verweist, * 
 * wird in einem struct abgelegt.			 */
struct mp3file* bytesToIdTag(char *buffer)
{
	/* HIER MUESST IHR EUREN CODE EINFUEGEN */

	return NULL;
}
/*				       *
 * Es werden die in dem struct mp3file *
 * abgelegten Informationen ausgegeben *
 *				       */
void printTag(struct mp3file *mp3)
{
	/* HIER MUESST IHR EUREN CODE EINFUEGEN */
}

/* Als Parameter ist entweder ein Integer oder ein Character zulaessig.	   *
 * Die Funktion wandelt den Code fuer ein Genre in einen lesbaren Text um. */
const char *genres[] = {"Blues",
                        "Classic Rock",
                        "Country",
                        "Dance",
                        "Disco",
                        "Funk",
                        "Grunge",
                        "Hip-Hop",
                        "Jazz",
                        "Metal",
                        "New Age",
                        "Oldies",
                        "Other",
                        "Pop",
                        "Rhythm and Blues",
                        "Rap",
                        "Reggae",
                        "Rock",
                        "Techno",
                        "Industrial",
                        "Alternative",
                        "Ska",
                        "Death Metal",
                        "Pranks",
                        "Soundtrack",
                        "Euro-Techno",
                        "Ambient",
                        "Trip-Hop",
                        "Vocal",
                        "Jazz&Funk",
                        "Fusion",
                        "Trance",
                        "Classical",
                        "Instrumental",
                        "Acid",
                        "House",
                        "Game",
                        "Sound Clip",
                        "Gospel",
                        "Noise",
                        "Alternative Rock",
                        "Bass",
                        "Soul",
                        "Punk",
                        "Space",
                        "Meditative",
                        "Instrumental Pop",
                        "Instrumental Rock",
                        "Ethnic",
                        "Gothic",
                        "Darkwave",
                        "Techno-Industrial",
                        "Electronic",
                        "Pop-Folk",
                        "Eurodance",
                        "Dream",
                        "Southern Rock",
                        "Comedy",
                        "Cult",
                        "Gangsta",
                        "Top 40",
                        "Christian Rap",
                        "Pop/Funk",
                        "Jungle",
                        "Native US",
                        "Cabaret",
                        "New Wave",
                        "Psychedelic",
                        "Rave",
                        "Showtunes",
                        "Trailer",
                        "Lo-Fi",
                        "Tribal",
                        "Acid Punk",
                        "Acid Jazz",
                        "Polka",
                        "Retro",
                        "Musical",
                        "Rock & Roll",
                        "Hard Rock",
                        "Folk"};
const char *translateGenre(int genre_id)
{
	if (genre_id >= 0 && genre_id <= 80) {
		return genres[genre_id];
	} else {
		return "undefined";
	}
}
