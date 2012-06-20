/*
 * mem.h
 * Memory management macros for kiceberg.
 * $Id$
 */

#include <err.h>

#ifdef TRACK_ALLOCATION
#define TRACK_MALLOC(pointer, structdesc)				\
		fprintf(stderr, "***** MALLOC'ed " structdesc		\
		    " %08lx\n", pointer);
#define TRACK_FREE(pointer, structdesc)					\
		fprintf(stderr, "***** FREE'ed " structdesc		\
		    " %08lx\n", pointer);
#else
#define	TRACK_MALLOC(pointer, structdesc)
#define	TRACK_FREE(pointer, structdesc)
#endif

#define MALLOC(pointer, structure, structdesc)				\
	{								\
		if ((pointer = (struct structure *)			\
		    malloc(sizeof(struct structure))) == NULL)		\
			errx(1, "Can't allocate " structdesc);		\
		TRACK_MALLOC(pointer, structdesc);			\
	}

#define FREE(pointer, structdesc)					\
	{								\
		free(pointer);						\
		TRACK_FREE(pointer, structdesc);			\
	}
