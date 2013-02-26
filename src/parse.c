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
 * parse.c
 * Recursive-descent parser for kiceberg.
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scan.h"
#include "parse.h"
#include "symbol.h"

#include "graph.h"
#include "rule.h"

extern struct symbol_table	*gstab;
extern struct legislature	*glaw;

/*** GRAPH ***/

void
kangaroo_iceberg(struct scan_st *sc, struct node *global)
{
	struct symbol *name;

	while (tokis(sc, TOKEN_IDENTIFIER)) {
		name = top_level_node(sc);
		node_arc_add(global, name, 0);
	}
}

struct symbol *
top_level_node(struct scan_st *sc)
{
	struct symbol *sym = NULL;
	struct node *n;

	sym = node_name(sc, gstab, SYM_LOOKUP_DEFINE);
	n = node_lit(sc, sym);
        n = n;

	return(sym);
}

struct symbol *
node_name(struct scan_st *sc, struct symbol_table *stab, int lookup)
{
	struct symbol *sym;
	int type = SYM_TYPE_NODE;

	sym = symbol_lookup(stab, sc->token);

	if (lookup == SYM_LOOKUP_UNIQUE) {
		if (sym != NULL) {
			scan_error(sc, "Symbol '%s' already defined", sc->token);
			scan(sc);
			return(NULL);
		} else {
			sym = symbol_define(stab, sc->token, type);
			scan(sc);
			return(sym);
		}
	} else if (lookup == SYM_LOOKUP_DEFINE) {
		if (sym != NULL) {
			scan(sc);
			return(sym);
		} else {
			sym = symbol_define(stab, sc->token, type);
			scan(sc);
			return(sym);
		}
	} else if (lookup == SYM_LOOKUP_EXTANT) {
		if (sym == NULL) {
			scan_error(sc, "Symbol '%s' not defined", sc->token);
			scan(sc);
			return(NULL);
		} else {
			scan(sc);
			return(sym);
		}
	}

	return(NULL);
}

struct node *
node_lit(struct scan_st *sc, struct symbol *sym)
{
	struct node *n = NULL;
	struct ruleset *rs = NULL;

	scan_expect(sc, "{");
	n = node_new(sym);
	while (tokne(sc, "/") && tokne(sc, "}") && !tokis(sc, TOKEN_EOF)) {
		arc_defn(sc, n);
	}
	if (tokeq(sc, "/")) {
		scan(sc);
		rs = ruleset(sc, sym);
		while(tokeq(sc, ";")) {
			scan(sc);
			rs = ruleset(sc, sym);
		}
	}
	scan_expect(sc, "}");
	rs = rs;
        
	return(n);
}	

void
arc_defn(struct scan_st *sc, struct node *n)
{
	struct symbol *name;
	int weight;

	name = node_ref(sc);
	scan_expect(sc, ":");
	weight = weight_defn(sc);
	node_arc_add(n, name, weight);
}

struct symbol *
node_ref(struct scan_st *sc)
{
	struct symbol *sym = NULL;
	struct node *dest = NULL;

	if (tokeq(sc, "{")) {
		sym = symbol_new_anon(gstab, SYM_TYPE_NODE);
		dest = node_lit(sc, sym);
	} else if (tokeq(sc, "^")) {
		scan(sc);
		sym = node_name(sc, gstab, SYM_LOOKUP_DEFINE);
	} else if (tokis(sc, TOKEN_IDENTIFIER)) {
		sym = node_name(sc, gstab, SYM_LOOKUP_UNIQUE);
		dest = node_lit(sc, sym);
	} else {
		scan_error(sc, "Expected identifier, `^', or `{'");
	}
        dest = dest;
	return(sym);
}

int
weight_defn(struct scan_st *sc)
{
	int weight;

	weight = atoi(sc->token);
	scan(sc);

	return(weight);
}

/*** RULES ***/

struct ruleset *
ruleset(struct scan_st *sc, struct symbol *sym)
{
	struct ruleset *rs;

	rs = ruleset_new(glaw, sym);
	rule(sc, rs);
	while(tokeq(sc, ",")) {
		scan(sc);
		rule(sc, rs);
	}

	ruleset_dump(rs);

	return(rs);
}

void
rule(struct scan_st *sc, struct ruleset *rs)
{
	struct rule *r;

	r = rule_new(rs);
	while(tokne(sc, "->") && !tokis(sc, TOKEN_EOF)) {
		arc_app(sc, r, PATTERN_ADD_MATCH);
	}
	scan_expect(sc, "->");
	while(tokne(sc, ",") && tokne(sc, ";") && tokne(sc, "}") &&
	      !tokis(sc, TOKEN_EOF)) {
		arc_app(sc, r, PATTERN_ADD_REWRITE);
	}
}

void
arc_app(struct scan_st *sc, struct rule *r, int which_side)
{
	struct symbol *name;
	int weight;

	name = node_app(sc);
	scan_expect(sc, ":");
	weight = weight_app(sc);
	rule_pattern_add(which_side, r, name, weight);
}

struct symbol *
node_app(struct scan_st *sc)
{
	struct symbol *name = NULL;

	if (tokeq(sc, "$")) {
		var_name(sc);
	} else {
		name = node_ref(sc);
	}
	
	return(name);
}

int
weight_app(struct scan_st *sc)
{
	int weight = 0;

	if (tokeq(sc, "$")) {
		var_name(sc);
	} else {
		weight = weight_defn(sc);
	}
	
	return(weight);
}

void
var_name(struct scan_st *sc)
{
	scan_expect(sc, "$");
	scan(sc);
}
