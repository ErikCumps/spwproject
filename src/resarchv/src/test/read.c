/** \file
 * Resource Archive - test code.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"

#define	BSIZE	4096

void
write_item (arc_item_t *item)
{
	int		fd;
	char		fname[64];
	unsigned long	todo;
	char		*p;
	ssize_t		got;

	memset (fname, 0, sizeof (fname));
	snprintf (fname, sizeof (fname) - 1, "item_%lu.resource", item->id);

	fd = open (fname, O_WRONLY|O_CREAT|O_TRUNC|O_BINARY, 0666);
	if (fd == -1) return;

	todo = item->size;
	p = (char *)item->data;
	while (todo > BSIZE) {
		got = write (fd, p, BSIZE);
		if (got != BSIZE) {
			printf ("*** Abort: failed to write data!\n");
			abort();
		}
		p += BSIZE; todo -= BSIZE;
	}
	if (todo) {
		got = write (fd, p, todo);
		if (got != (ssize_t)todo) {
			printf ("*** Abort: failed to write data!\n");
			abort();
		}
	}

	close (fd);

	printf ("wrote item (%lu) to file \"%s\".\n", item->id, fname);
}

int
main (int argc, char *argv[])
{
	arc_t		*arc = NULL;
	arc_item_t	*item = NULL;
	//int		first	= 0;
	unsigned long	id;

	if (argc > 1)
		arc = arc_load (argv[1], NULL);
	else
		arc = arc_load (outfile, (arc_sig_t *)signature);

	if (arc == NULL) {
		printf ("*** Error: failed to open archive file \"%s\"!\n", outfile);
		exit (42);
	}

	printf ("%6s %10s %10s %s\n", "ID", "POS", "SIZE", "NAME");
	printf ("%6s %10s %10s %s\n", "--", "---", "----", "----");
	while ((item = arc_next (arc, item)) != NULL) {
		printf ("%6lu %10lu %10lu %s\n", item->id, item->pos, item->size, item->name);
		//if (!first) {
		//	first = 1;
		//	write_item (item);
		//}
	}
	printf ("\n");

	id = 101;
	item = arc_byid (arc, id);
	if (item == NULL)
		printf ("*** Error: item with id %lu not found!\n", id);
	else {
		printf ("%6lu %10lu %10lu %s\n", item->id, item->pos, item->size, item->name);
		write_item (item);
		printf ("\n");
	}

	return (0);
}
