/*
 * symbol.h
 * Symbol structures and prototypes for kiceberg.
 * $Id$
 */

#ifndef __SYMBOL_H
#define __SYMBOL_H

/*
 * This is just so we don't have to include graph.h
 * because we don't want a circular dependency.
 */

struct node;

/*
 * Along with the global symbol table, individual symbol
 * tables exist for each rule (for local bindings.)
 */
struct symbol_table {
	struct symbol	*head;
};

struct symbol {
	char		*token;		/* lexeme making up the symbol */
	int		 type;		/* SYM_TYPE_*, below */
	struct symbol	*next;		/* next symbol in symbol table */
	/*
	 * for SYM_TYPE_NODE symbols, this points
	 * to the node that is named by this symbol.
	 */
	struct node	*dest;
};

#define SYM_TYPE_NODE		1	/* symbol refers to a node */

#define SYM_LOOKUP_UNIQUE	0	/* symbol must not already exist */
#define SYM_LOOKUP_DEFINE	1	/* symbol may or may not already exist */
#define SYM_LOOKUP_EXTANT	2	/* symbol must already exist */

struct symbol_table	*symbol_table_new(void);
void			 symbol_table_free(struct symbol_table *);
struct symbol		*symbol_new_anon(struct symbol_table *, int);
struct symbol		*symbol_define(struct symbol_table *, char *, int);
struct symbol		*symbol_lookup(struct symbol_table *, char *);
void			 symbol_free(struct symbol *);

void			 symbol_table_dump(int, struct symbol_table *);
void			 symbol_print(struct symbol *);
void			 symbol_dump(int, struct symbol *);

#endif /* !__SYMBOL_H */
