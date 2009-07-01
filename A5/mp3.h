#ifndef _MP3_H_
#define _MP3_H_

struct mp3file {
	char interpret[31];
	char titel[31];
	char album[31];
	char kommentar[31];
	char jahr[5];
	char genre;
};

void idTagFile(const char *,char *);
void idTagDir(const char *,char *);
struct mp3file *bytesToIdTag(char *);
void printTag(struct mp3file *);
const char *translateGenre(int genre_id);

#endif
