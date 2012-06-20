/*
 * Copyright (c)2004 Cat's Eye Technologies.  All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 
 *   Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * 
 *   Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * 
 *   Neither the name of Cat's Eye Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

/*
 * main.c
 * kiceberg - Reference interpreter for
 * "The Kangaroo Iceberg Programming Language"
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "scan.h"
#include "parse.h"
#include "symbol.h"

#include "graph.h"
#include "rule.h"

/* GLOBALS */

struct symbol_table	*gstab;	/* general symbol table */
struct legislature	*glaw;	/* all rules */

/* PROTOTYPES */

void			 usage(void);

/* FUNCTIONS */

int
main(int argc, char **argv)
{
	struct scan_st *sc;
	int do_dump = 0;
	struct node *global;
	int argn;
	char *global_ident;

	/* Initialize / allocate globals. */

	gstab = symbol_table_new();
	global_ident = malloc(20 * sizeof(char));
	strcpy(global_ident, "__GLOBAL__");
	global = node_new(symbol_define(gstab, global_ident, SYM_TYPE_NODE));
	glaw = legislature_new();

	/* Parse arguments. */

	for (argn = 1; argn < argc; argn++) {
		if (argv[argn][0] == '-') {
			switch(argv[argn][1]) {
			case 'd':
				do_dump = 1;
				break;
			default:
				usage();
			}
		} else {
			break;
		}
	}
	
	/* Parse the input file. */

	while (argn < argc) {
		sc = scan_open(argv[0]);
		kangaroo_iceberg(sc, global);
		scan_close(sc);
		argn++;
	}

	if (do_dump) {
		fprintf(stderr, "*** Symbol Table ***\n");
		symbol_table_dump(0, gstab);
		fprintf(stderr, "\n");
		fprintf(stderr, "*** Global Graph Node ***\n");
		graph_dump(global);
		fprintf(stderr, "*** All Rulesets ***\n");
		legislature_dump(glaw);
		fprintf(stderr, "\n");
	}

	/* symbol_table_free(gstab); */
	exit(0);
}

void
usage(void)
{
	fprintf(stderr, "usage: kiceberg [-d] srcfile ...\n");
	exit(1);
}
