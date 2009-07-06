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
    struct mp3file *mfile = NULL;
    struct stat finfo = {0};
    char buf[128];
    size_t rb = 0;
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
        /* falls ein Verzeichnis, rekursiv durchsuchen */
        if (S_ISDIR(finfo.st_mode))
        {
            idTagDir(fileName, NULL);
            return;
        }
        else
        {
            fprintf(stderr, "Keine regulaere Datei: %s\n", fileName);
            return;
        }
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

    /* Tag auslesen */
    if ((rb = fread(buf, sizeof(char), ID3_SIZE, datei)) != ID3_SIZE)
    {
        fprintf(stderr, "Fehler beim Lesen der Datei: %s\n", fileName);
        fclose(datei);
        return;
    }

    /* Datei-Handle schliessen */
    fclose(datei);

#ifdef DEBUG
        fprintf(stderr, "%d Bytes erfolgreich gelesen.\n", (int)rb);
#endif

    /* Tags parsen und printen */
    if ((mfile = bytesToIdTag(buf)) != NULL)
    {
        printTag(mfile);
        free(mfile);
    }
    else
    {
        fprintf(stderr, "Kein ID3-Tag hinterlegt: %s\n", fileName);
    }
}

void idTagDir(const char *dirName, char *comment)
{
    struct dirent *ent = NULL;
    DIR *dir = NULL;
    char fileName[256];

    if (!strncmp(".", dirName, 2) || !strncmp("..", dirName, 3))
        return;
    
    /* Verzeichnis oeffnen */
    if (!(dir = opendir(dirName)))
    {
        fprintf(stderr, "Konnte Verzeichnis nicht oeffnen: %s\n", dirName);
        return;
    }

    printf("%s: Betrete Verzeichnis ...\n", dirName);

    /* Verzeichnis durchlaufen und ueberpruefen */
    while ((ent = readdir(dir)))
    {
        if (strncmp(".", ent->d_name, 2) && strncmp("..", ent->d_name, 3))
        {
            if ((strlen(dirName) + strlen(ent->d_name)) >= 255)
            {
                fprintf(stderr, "Dateiname zu lang.\n");
                continue;
            }

            sprintf(fileName, "%s/%s", dirName, ent->d_name);

            idTagFile(fileName, NULL);
        }
    }

    printf("%s: Verlasse Verzeichnis ...\n", dirName);

    /* Verzeichnis-Handle schliessen */
    if ((closedir(dir)) == -1)
    {
        fprintf(stderr, "Konnte Verzeichnis nicht ordentlich verlassen: %s\n",
                dirName);
        return;
    }
}

/* Die Informationen im Puffer, auf den buffer verweist, * 
 * wird in einem struct abgelegt.            */
struct mp3file* bytesToIdTag(char *buffer)
{
    char *ptr = buffer;
    struct mp3file *mp3;

    /* Pruefen, ob ID3-Tag hinterlegt */
    if (strncmp("TAG", buffer, 3))
        return NULL;

    mp3 = (struct mp3file *) malloc(sizeof(struct mp3file));

    if (mp3 == NULL)
    {
        fprintf(stderr, "Konnte keinen Speicher reservieren.\n");
        exit(-1);
    }

    /* buffer auslesen */
    ptr += 3;
    strncpy(mp3->titel, ptr, 30);
    mp3->titel[30] = '\0';

    ptr += 30;
    strncpy(mp3->interpret, ptr, 30);
    mp3->interpret[30] = '\0';

    ptr += 30;
    strncpy(mp3->album, ptr, 30);
    mp3->album[30] = '\0';
    
    ptr += 30;
    strncpy(mp3->jahr, ptr, 4);
    mp3->jahr[4] = '\0';

    ptr += 4;
    strncpy(mp3->kommentar, ptr, 30);
    mp3->kommentar[30] = '\0';

    ptr += 30;
    mp3->genre = *ptr;

    return mp3;
}
/*                     *
 * Es werden die in dem struct mp3file *
 * abgelegten Informationen ausgegeben *
 *                     */
void printTag(struct mp3file *mp3)
{
    printf("Interpret: %s\n", mp3->interpret);
    printf("Titel:     %s\n", mp3->titel);
    printf("Album:     %s\n", mp3->album);
    printf("Kommentar: %s\n", mp3->kommentar);
    printf("Jahr:      %s\n", mp3->jahr);
    printf("Genre:     %s\n", translateGenre(mp3->genre));
    printf("---------------------------------\n");
}

/* Als Parameter ist entweder ein Integer oder ein Character zulaessig.    *
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
    if (genre_id >= 0 && genre_id <= 80)
        return genres[genre_id];
    else
        return "undefined";
}
