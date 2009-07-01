CC=gcc
FLAGS=-Wall -D_XOPEN_SOURCE -D_POSIX_SOURCE
FLAGS_EVIL=-Werror -pedantic -ansi

all: mp3

mp3: main.c mp3.h mp3.c
	$(CC) $(FLAGS) -o mp3 main.c mp3.c

mp3_debug: main.c mp3.h mp3.c
	$(CC) $(FLAGS) -O0 -g -o mp3 main.c mp3.c

mp3_evil: main.c mp3.h mp3.c
	$(CC) $(FLAGS) $(FLAGS_EVIL) -o mp3 main.c mp3.c

clean: 
	rm -f mp3

.PHONY: clean
