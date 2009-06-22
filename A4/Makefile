CC=gcc
CFLAGS=-ansi -pedantic -D_POSIX_SOURCE -D_XOPEN_SOURCE -Wall
CFLAGS+=-DDEBUG -g

malloctest: malloctest.c firstfit.c firstfit.h
	$(CC) $(CFLAGS) -o malloctest malloctest.c firstfit.c

clean:
	rm -f malloctest

.PHONY: clean
