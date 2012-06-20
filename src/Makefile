# BSD Makefile for kiceberg.
# $Id$

CC=	gcc
CFLAGS=	-Wall -ansi -pedantic
LIBS=	

CFLAGS+= ${EXTRA_CFLAGS}

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
