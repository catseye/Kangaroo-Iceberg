/*
 * rule.h
 * Rule structures and prototypes for kiceberg.
 * $Id$
 */

#ifndef __RULE_H
#define __RULE_H

#include "symbol.h"

struct legislature {
	struct ruleset		*head;
};

struct ruleset {
	struct ruleset		*next;
	struct rule		*head;
	struct symbol		*loc;	/* symbol of node we're in */
};

struct rule {
	struct rule		*next;
	struct pattern		*match;
	struct pattern		*rewrite;
	struct symbol_table	*params;
	int			priority;
};

struct pattern {
	struct pattern		*next;
	struct symbol		*name;	/* symbol of node that arc points to */
	int			 weight;/* weight of arc (match or rewrite) */
};

#define PATTERN_ADD_MATCH	1
#define PATTERN_ADD_REWRITE	2

struct legislature	*legislature_new(void);
struct ruleset		*ruleset_new(struct legislature *, struct symbol *);
struct rule		*rule_new(struct ruleset *);

struct pattern		*rule_pattern_add(int, struct rule *, struct symbol *, int);

void			 legislature_dump(struct legislature *);
void			 ruleset_dump(struct ruleset *);
void			 rule_dump(struct rule *);
void			 pattern_dump(struct pattern *);

#endif /* !__RULE_H */
