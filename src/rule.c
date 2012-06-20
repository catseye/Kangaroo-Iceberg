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
 * rule.c
 * Rule housekeeping and manipulation for kiceberg.
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>

#include "rule.h"
#include "symbol.h"
#include "mem.h"

struct legislature *
legislature_new(void)
{
	struct legislature *l;

	MALLOC(l, legislature, "legislature");
	l->head = NULL;
	return(l);
}

struct ruleset *
ruleset_new(struct legislature *l, struct symbol *loc)
{
	struct ruleset *rs;

	MALLOC(rs, ruleset, "ruleset");
	rs->head = NULL;
	rs->loc = loc;

	rs->next = l->head;
	l->head = rs;

	return(rs);
}

struct rule *
rule_new(struct ruleset *rs)
{
	struct rule *r;

	MALLOC(r, rule, "rule");

	r->match = NULL;
	r->rewrite = NULL;
	r->params = NULL;

	r->next = rs->head;
	rs->head = r;
	return(r);
}

/*
 * This doesn't handle free variables yet.
 */
struct pattern *
rule_pattern_add(int which_side, struct rule *r, struct symbol *name, int weight)
{
	struct pattern *p;

	MALLOC(p, pattern, "pattern");

	p->name = name;
	p->weight = weight;

	if (which_side == PATTERN_ADD_MATCH) {
		p->next = r->match;
		r->match = p;
	} else if (which_side == PATTERN_ADD_REWRITE) {
		p->next = r->rewrite;
		r->rewrite = p;
	}
	
	return(p);
}

void
legislature_dump(struct legislature *l)
{
	struct ruleset *rs;

	for (rs = l->head; rs != NULL; rs = rs->next) {
		ruleset_dump(rs);
	}
}

void
ruleset_dump(struct ruleset *rs)
{
	struct rule *r;

	if (rs == NULL) {
		fprintf(stderr, "*NULLRULESET*\n");
		return;
	}

	for (r = rs->head; r != NULL; r = r->next) {
		rule_dump(r);
		fprintf(stderr, ", ");
	}
	fprintf(stderr, ";\n");
}

void
rule_dump(struct rule *r)
{
	struct pattern *p;

	if (r == NULL) {
		fprintf(stderr, "*NULLRULE*\n");
		return;
	}

	for (p = r->match; p != NULL; p = p->next) {
		pattern_dump(p);
		fprintf(stderr, " ");
	}
	fprintf(stderr, "-> ");
	for (p = r->rewrite; p != NULL; p = p->next) {
		pattern_dump(p);
		fprintf(stderr, " ");
	}
}

void
pattern_dump(struct pattern *p)
{
	if (p == NULL) {
		fprintf(stderr, "*NULLPATTERN*\n");
		return;
	}

	symbol_print(p->name);
	fprintf(stderr, ":%d", p->weight);
}
