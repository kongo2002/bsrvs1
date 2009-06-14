# Variablendefinieren
# Welchen Compiler verwenden wir?
CC=gcc
# Welche Flags sollen gesetzt werden?
CFLAGS=-Wall
# Optional die harten Falgs
CFLAGS_HARD=-Werror -ansi -pedantic -D_XOPEN_SOURCE -D_POSIX_SOURCE

# Targets
kreuzung: kreuzung.c sync.c sync.h
	$(CC) $(CFLAGS) -o kreuzung kreuzung.c sync.c

kreuzungA: kreuzungA.c sync.c sync.h
	$(CC) $(CFLAGS) -o kreuzungA kreuzungA.c sync.c

kreuzung_hard: kreuzung.c sync.c sync.h
	$(CC) $(CFLAGS) $(CFLAGS_HARD) -o kreuzung kreuzung.c sync.c

clean:
	rm -f kreuzung

.PHONY: clean
