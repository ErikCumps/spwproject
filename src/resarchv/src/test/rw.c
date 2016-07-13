/** \file
 * Resource Archive - test code.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"

#define	BSIZE	4096

int
main (int argc, char *argv[])
{
	arc_t		*arc = NULL;
	arc_item_t	*item = NULL;
	unsigned long	id;

	if (argc > 1)
		arc = arc_load (argv[1], NULL);
	else {
		printf ("Usage: %s filename\n", argv[0]);
		exit (0);
	}

	if (arc == NULL) {
		printf ("*** Error: failed to open archive file \"%s\"!\n", argv[1]);
		exit (42);
	}

	printf ("%6s %10s %10s %s\n", "ID", "POS", "SIZE", "NAME");
	printf ("%6s %10s %10s %s\n", "--", "---", "----", "----");
	while ((item = arc_next (arc, item)) != NULL) {
		printf ("%6lu %10lu %10lu %s\n", item->id, item->pos, item->size, item->name);
	}
	printf ("\n");

	id = 101;
	item = arc_byid (arc, id);
	if (item == NULL)
		printf ("*** Error: item with id %lu not found!\n", id);
	else {
		printf ("%6lu %10lu %10lu %s\n", item->id, item->pos, item->size, item->name);
		printf ("\n");

		arc_next (arc, NULL);
		arc_add (arc, item->id, item->name);
		arc_store (arc);
	}

	return (0);
}
