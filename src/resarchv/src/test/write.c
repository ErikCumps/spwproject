/** \file
 * Resource Archive - test code.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "common.h"

#include <ctype.h>
#include <errno.h>

int
parse_cmd (char *line, arc_t *arc)
{
	char		*p, *q;
	unsigned long	id;
	char		*name;
	int		len, rc;

	if ((line == NULL) || (arc == NULL)) abort();

	p = line; while (isspace (*p)) p++;
	if (!isdigit (*p)) return (0);

	/* use strtoul side-effect to get to next field on line */
	errno = 0; id = strtoul (p, &q, 0);
	if (errno || !isspace (*q)) return (0);

	/* skip whitespace */
	p = q; while (isspace (*p)) p++;
	if (*p != '"') return (0);

	/* extract filename */
	p++; q = p; while ((*p != '\0') && (*p != '"')) p++;
	if (*p != '"') return (0);
	*p = '\0';
	name = strdup (q);

	/* fix backslashes in filename */
	p = name; len = strlen (p);
	while (*p != '\0') {
		if (*p == '\\') {
			*p = '/'; p++;
			/* calc len - offset, offset == p - item->name, because we shift p
			 * with 1 char we lose the extra '\' and include the trailing '\0'! */
			memmove (p, p+1, len - (p - name));
			/* length is reduced by 1! */
			len--;
		} else p++;
	}

	rc = arc_add (arc, id, name);

	if (rc) printf ("%lu\t%s\n", id, name);

	free (name);
	return (rc);
}

int
parse_cfg (char *file, arc_t *arc)
{
	FILE	*cfg;
	char	buffer[256];
	int	cnt = 0;

	cfg = fopen (file, "r");
	if (cfg == NULL) {
		printf ("*** Error: failed to open file \"%s\" for reading!\n", file);
		exit (42);
	}

	while (fgets (buffer, sizeof (buffer), cfg) != NULL) {
		cnt += parse_cmd (buffer, arc);
	}

	fclose (cfg);

	return (cnt);
}

int main (int argc, char *argv[])
{
	int	count = 0;
	arc_t	*arc;

	if (argc < 2) {
		printf ("Usage: %s cfgfile\n\n", argv[0]);
		exit (1);
	}

	arc = arc_new (outfile, (arc_sig_t *)signature);
	if (arc == NULL) {
		printf ("*** Error: failed to create empty archive!\n");
		exit (42);
	}

	count = parse_cfg (argv[1], arc);
	printf ("Config file \"%s\" contains %u items.\n", argv[1], count);

	arc_store (arc);

	return (0);
}
