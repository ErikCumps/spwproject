/** \file
 * Resource Archive - report tool.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "report.h"

#define	BSIZE	4096

static int	o_dump = 0;
static char	o_file[MAX_PATH+1];

void
usage (char *app)
{
	printf ("Usage: %s [-d] file\n", app);
	printf ("Where: file   resource file\n");
	printf ("       -d     dump resource items\n");
	printf ("\n");
	exit (0);
}

void
parse_args (int argc, char **argv)
{
	int	i = 1;

	if (argc <= 1) usage(argv[0]);

	memset (o_file, 0, sizeof (o_file));

	if (strcmp (argv[i], "-d") == 0) {
		o_dump = 1;
		i++;
	}

	snprintf (o_file, sizeof (o_file) - 1, "%s", argv[i]);
}

void
dump_item (arc_item_t *item)
{
	int		fd;
	char		fname[64];
	unsigned long	todo;
	char		*p;
	ssize_t		got;

	memset (fname, 0, sizeof (fname));
	snprintf (fname, sizeof (fname) - 1, "item_%04lu.resource", item->id);

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

	parse_args (argc, argv);

	arc = arc_load (o_file, NULL);
	if (arc == NULL) {
		printf ("*** Error: failed to open archive file \"%s\"!\n", o_file);
		exit (42);
	}

	printf ("%6s %-12s %10s %10s %s\n", "ID", "TYPE", "POS", "SIZE", "NAME");
	printf ("%6s %-12s %10s %10s %s\n", "--", "----", "---", "----", "----");
	while ((item = arc_next (arc, item)) != NULL) {
		printf ("%6lu %-12s %10lu %10lu %s\n", item->id, arc_type_name (item->type), item->pos, item->size, item->name);
		if (o_dump) dump_item (item);
	}
	printf ("\n");

	arc_close (arc);

	return (0);
}
