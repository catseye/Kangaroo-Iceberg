# Makefile for kiceberg.
# $Id$

CC?=gcc
LIBS?=

WARNS=	-Werror -W -Wstrict-prototypes -Wmissing-prototypes \
	-Wpointer-arith	-Wno-uninitialized -Wreturn-type -Wcast-qual \
	-Wwrite-strings -Wswitch -Wshadow -Wcast-align \
	-Winline -Wnested-externs -Wredundant-decls

# Won't compile under ANSI, but for consistency with other Makefiles:
ifdef ANSI
  CFLAGS+= -ansi -pedantic
else
  CFLAGS+= -std=c99 -D_POSIX_C_SOURCE=200112L
endif

CFLAGS+= ${WARNS} ${EXTRA_CFLAGS}

OBJS=	graph.o rule.o \
	symbol.o \
	scan.o parse.o \
	main.o

all: kiceberg

kiceberg: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o kiceberg $(LIBS)

rule.o: rule.c rule.h symbol.h mem.h
	$(CC) $(CFLAGS) -c rule.c -o rule.o

graph.o: graph.c graph.h symbol.h mem.h
	$(CC) $(CFLAGS) -c graph.c -o graph.o

symbol.o: symbol.c symbol.h mem.h
	$(CC) $(CFLAGS) -c symbol.c -o symbol.o

scan.o: scan.c scan.h mem.h
	$(CC) $(CFLAGS) -c scan.c -o scan.o

parse.o: parse.c parse.h scan.h symbol.h
	$(CC) $(CFLAGS) -c parse.c -o parse.o

main.o: main.c scan.h parse.h
	$(CC) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f *.o *.core kiceberg
