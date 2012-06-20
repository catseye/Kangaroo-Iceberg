/*
 * scan.h
 * Lexical scanner structures and prototypes for kiceberg.
 * $Id$
 */

#ifndef __SCAN_H
#define __SCAN_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct scan_st {
	FILE	*in;
	char	*token;
	int	 toktype;
	int	 lino;
	int	 columno;
};

#define	TOKEN_EOF		-1
#define	TOKEN_NONE		 0
#define	TOKEN_SYMBOL		 1
#define	TOKEN_IDENTIFIER	 2
#define	TOKEN_NUMBER		 3

#define tokeq(sc, x)	(!strcmp(sc->token, x))
#define tokne(sc, x)	(strcmp(sc->token, x))
#define tokis(sc, x)	(sc->toktype == x)

extern struct scan_st *scan_open(char *);
extern void scan_close(struct scan_st *);
extern void scan_error(struct scan_st *, const char *, ...);
extern void scan(struct scan_st *);
extern void scan_expect(struct scan_st *, const char *);

#endif /* !__SCAN_H */
