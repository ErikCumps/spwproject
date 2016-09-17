/** \file
 * A simple tool to track a build number.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"

static void
usage (char *app)
{
	printf ("Usage: %s dbfile cfile\n", app);
	printf ("Where: dbfile   name of BUILD database file\n");
	printf ("       cfile    name of C source file\n");
	printf ("\n");
	exit (0);
}

static void
parse_args (int argc, char **argv, char **dfile, char **cfile)
{
	FILE	*f;

	*dfile = *cfile = NULL;

	if (argc < 3) usage (argv[0]);

	*dfile = strdup (argv[1]);
	*cfile = strdup (argv[2]);

	if (!*dfile || !*cfile) usage (argv[0]);
}

static int
safe_strtoul (const char *str, unsigned long *result)
{
	unsigned long 	ul;
	char		*ep;
	int		en;

	*result = 0;
        errno = 0;
	ul = strtoul (str, &ep, 0);
	en = errno;

	if ((str[0] == '\0') || (ep[0] != '\0')) {
		/* note that errno is 0 in this case */
		return (0);
	}
	if (en == ERANGE) {
		return (0);
	}

	*result = ul;

	return (1);
}

static void
update_bid (char *dbf, unsigned long *bid)
{
	FILE		*f;
	char		buff[64], *p;
	unsigned long	data;

	*bid = data = 0;

	if ((f = fopen (dbf, "r")) != NULL) {
		memset (buff, 0, sizeof (buff));
		fgets (buff, sizeof (buff), f);
		fclose (f);

		if (buff[0] != '\0') {
			if ((p = strchr (buff, '\n')) !=NULL ) *p = '\0';
			safe_strtoul (buff, &data);
		}
	}

	data++;

	if ((f = fopen (dbf, "w")) != NULL) {
		fprintf (f, "%lu\n", data);
		fclose (f);
	}

	*bid = data;
}

static void
prepare_src (char *cfile)
{
	char	ftpl[MAX_PATH+1], fsrc[MAX_PATH+1];
	FILE	*fi, *fo;
	char	buf[4096];

	memset (ftpl, 0, sizeof (ftpl));
	snprintf (ftpl, sizeof (ftpl) - 1, "%s.template", cfile);

	memset (fsrc, 0, sizeof (fsrc));
	snprintf (fsrc, sizeof (fsrc) - 1, "%s", cfile);

	fo = fopen (fsrc, "r");
	if (fo) {
		fclose (fo);
		return;
	}

	fo = fopen (fsrc, "w");
	fi = fopen (ftpl, "r");

	if (!fi || !fo) exit (1);

	while (fgets (buf, sizeof (buf), fi)) {
		fprintf (fo, "%s", buf);
	}
	fclose (fi);
	fclose (fo);
}

static void
fix_string (char *str, unsigned long bid)
{
	char	*srch = "BLD_BUILD = ";
	char	*p, *q;
	char	buf[4096];

	memcpy (buf, str, sizeof (buf));

	p = strstr (buf, srch);
	if (!p) return;

	p += strlen (srch);
	if (*p != '"') return;

	p++; q = p; p = strchr (q, '"');
	if (!p) return;

	*q = '\0';

	snprintf (str, sizeof (buf) - 1, "%s%lu%s", buf, bid, p);
}

static void
update_src (char *cfile, unsigned long bid)
{
	char	fori[MAX_PATH+1], fnew[MAX_PATH+1], fbak[MAX_PATH+1];
	FILE	*fi, *fo;
	char	buf[4096];

	memset (fori, 0, sizeof (fori));
	memset (fnew, 0, sizeof (fnew));
	memset (fbak, 0, sizeof (fbak));
	snprintf (fori, sizeof (fori) - 1, "%s", cfile);
	snprintf (fnew, sizeof (fnew) - 1, "%s.new", cfile);
	snprintf (fbak, sizeof (fbak) - 1, "%s.bak", cfile);

	fi = fopen (fori, "r");
	fo = fopen (fnew, "w");

	if (!fi || !fo) exit (1);

	while (fgets (buf, sizeof (buf), fi)) {
		fix_string (buf, bid);
		fprintf (fo, "%s", buf);
	}
	fclose (fi);
	fclose (fo);

	if (rename (fori, fbak) < 0) {
		remove (fnew);
		exit (2);
	}
	if (rename (fnew, fori) < 0) {
		rename (fbak, fori);
		remove (fnew);
		exit (3);
	}
	remove (fbak);
}

int
main (int argc, char *argv[])
{
	char		*dfile = NULL;
	char		*cfile = NULL;
	unsigned long	bid = 0;

	parse_args (argc, argv, &dfile, &cfile);
	update_bid (dfile, &bid);
	prepare_src (cfile);
	update_src (cfile, bid);
}
