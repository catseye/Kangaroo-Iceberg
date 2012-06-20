/*
 * parse.h
 * Parser structures and prototypes for kiceberg.
 * $Id$
 */

#ifndef __PARSE_H
#define __PARSE_H

#include "scan.h"
#include "symbol.h"
#include "graph.h"
#include "rule.h"

void			 kangaroo_iceberg(struct scan_st *, struct node *);
struct symbol		*top_level_node(struct scan_st *);
struct symbol		*node_name(struct scan_st *, struct symbol_table *, int);
struct node		*node_lit(struct scan_st *, struct symbol *);
void			 arc_defn(struct scan_st *, struct node *);
struct symbol		*node_ref(struct scan_st *);
int			 weight_defn(struct scan_st *);
struct ruleset		*ruleset(struct scan_st *, struct symbol *);
void			 rule(struct scan_st *, struct ruleset *);
void			 arc_app(struct scan_st *, struct rule *, int);
struct symbol		*node_app(struct scan_st *);
int			 weight_app(struct scan_st *);
void			 var_name(struct scan_st *);

#endif /* !__PARSE_H */
