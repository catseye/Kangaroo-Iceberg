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
 * graph.h
 * Graph housekeeping and manipulation for kiceberg.
 * $Id$
 */

#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "symbol.h"
#include "mem.h"

struct node *
node_new(struct symbol *sym)
{
	struct node *n;

	MALLOC(n, node, "graph node");

	n->head = NULL;
	n->name = sym;

	if (sym != NULL)
		sym->dest = n;

	return(n);
}

void
node_free(struct node *n)
{
	struct arc *a, *a_last = NULL;

	a = n->head;
	while (a != NULL) {
		a_last = a;
		a = a->next;
		FREE(a_last, "arc");
	}
	FREE(n, "graph node");
}

void
graph_free(struct node *n)
{
	/* struct arc *a; */

	/* free all nodes connected to arcs */
	/* construct an itinerary and so forth */
	node_free(n);
}

struct arc *
node_arc_add(struct node *n, struct symbol *name, int weight)
{
	struct arc *a;

	MALLOC(a, arc, "graph arc");

	a->name = name;
	a->weight = weight;

	a->next = n->head;
	n->head = a;
	
	return(a);
}

struct itinerary *
itinerary_new(void)
{
	struct itinerary *i;

	MALLOC(i, itinerary, "itinerary");
	i->head = NULL;
	return(i);
}

void
itinerary_free(struct itinerary *i)
{
	struct visit *v, *v_last = NULL;

	v = i->head;
	while (v != NULL) {
		v_last = v;
		v = v->next;
		FREE(v_last, "visit");
	}
}

struct visit *
itinerary_add(struct itinerary *i, struct node *n)
{
	struct visit *v;

	MALLOC(v, visit, "visit");

	v->dest = n;

	v->next = i->head;
	i->head = v;

	return(v);
}

int
itinerary_find(struct itinerary *i, struct node *n)
{
	struct visit *v;

	for (v = i->head; v != NULL; v = v->next) {
		if (v->dest == n)
			return(1);
	}
	
	return(0);
}


void
node_dump(struct node *n)
{
	struct arc *a;

	if (n == NULL) {
		fprintf(stderr, " *NULLNODE { }\n");
		return;
	}
	symbol_print(n->name);
	fprintf(stderr, " { ");

	for (a = n->head; a != NULL; a = a->next)
		arc_dump(a);

	fprintf(stderr, "}\n");
}

void
arc_dump(struct arc *a)
{
	if (a == NULL) {
		fprintf(stderr, "*NULLARC*");
		return;
	}

	symbol_print(a->name);
	fprintf(stderr, ":%d ", a->weight);
}

void
graph_dump(struct node *start)
{
	struct itinerary *i;
		
	i = itinerary_new();
	graph_dump_r(start, i, 0);
	fprintf(stderr, "\n");
}

#define INDENT(x) { int j; for (j = 0; j < x; j++) fprintf(stderr, "  "); }

void
graph_dump_r(struct node *n, struct itinerary *i, int indent)
{
	struct arc *a;

	if (n == NULL)
		return;

	INDENT(indent);
	
	if (itinerary_find(i, n)) {
		symbol_print(n->name);
	} else {
		itinerary_add(i, n);
		symbol_print(n->name);
		fprintf(stderr, " {\n");
		for (a = n->head; a != NULL; a = a->next) {
			graph_dump_r(a->name->dest, i, indent + 1);
			fprintf(stderr, ":%d\n", a->weight);
		}
		INDENT(indent);
		fprintf(stderr, "}");
	}
}
