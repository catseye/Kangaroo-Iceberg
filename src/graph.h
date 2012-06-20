/*
 * graph.h
 * Graph structures and prototypes for kiceberg.
 * $Id$
 */

#ifndef __GRAPH_H
#define __GRAPH_H

#include "symbol.h"

struct node {
	struct arc		*head;
	struct symbol		*name;
};

struct arc {
	struct arc		*next;
	struct symbol		*name;	/* symbol of node that this arc points to */
	int			 weight;
};

struct itinerary {
	struct visit		*head;
};

struct visit {
	struct visit		*next;
	struct node		*dest;
};

struct node		*node_new(struct symbol *);
void			 node_free(struct node *);
void			 graph_free(struct node *);
struct arc		*node_arc_add(struct node *, struct symbol *, int);

struct itinerary	*itinerary_new(void);
void			 itinerary_free(struct itinerary *);
struct visit		*itinerary_add(struct itinerary *, struct node *);
int			 itinerary_find(struct itinerary *, struct node *);

void			 node_dump(struct node *);
void			 arc_dump(struct arc *);
void			 graph_dump(struct node *);
void			 graph_dump_r(struct node *, struct itinerary *, int);

#endif /* !__GRAPH_H */
