CC=gcc
CFLAGS=-Wall -ansi -pedantic -D_XOPEN_SOURCE -D_POSIX_SOURCE

all: primserv primeat

primserv: primserv.c primshm.c primshm.h sync.c sync.h
	$(CC) $(CFLAGS) -o primserv primserv.c primshm.c sync.c
	
primeat: primeat.c primshm.c primshm.h sync.c sync.h
	$(CC) $(CFLAGS) -o primeat primeat.c primshm.c sync.c
	
clean:
	rm -f primserv primeat *.o

test: all
	./primserv&
	sleep 2
	./primeat

.PHONY: clean test

