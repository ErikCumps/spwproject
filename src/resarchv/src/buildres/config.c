/** \file
 * Resource Archive - buildres tool - argument parsing.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <system/independence.h>
#include "config.h"
#include "util.h"

static char *def_outlib = "resource.res";
static char *def_outhdr = "resource.h";
static char *def_outsrc = "resource.c";

CONFIG	cfg;

static void
cfg_init (void)
{
	memset (&cfg, 0, sizeof (cfg));
	SAFE_STRDUP (def_outlib, &(cfg.outlib));
	SAFE_STRDUP (def_outhdr, &(cfg.outhdr));
	SAFE_STRDUP (def_outsrc, &(cfg.outsrc));
}

static void
cfg_strdup (char **v, char *d)
{
	if (*v) free (*v);

	SAFE_STRDUP (d, v);
}

void
usage (char *appname, char *msg)
{
	if (msg) {
		fprintf (stderr, "*** Error: %s\n\n", msg);
	}
	printf ("Usage: %s [-h] [-ol file] [-oh file] [-oc file] cfgfile\n", appname);
	printf ("Where: -ol file   output library filename (default \"%s\")\n", def_outlib);
	printf ("       -oh file   output header filename (default \"%s\")\n", def_outhdr);
	printf ("       -oc file   output source filename (default \"%s\")\n", def_outsrc);
	printf ("       cfgfile    configuration file\n");
	printf ("\n");
	exit (0);
}

void
parse_args (int c, char **v)
{
	char	*appname = v[0];

	c--; v++; cfg_init ();

	if (c == 0) usage (appname, NULL);

	while (c > 0) {
		if (v[0][0] != '-') {
			if (cfg.cfg == NULL)
				cfg_strdup (&cfg.cfg, v[0]);
			else
				usage (appname, "duplicate cfgfile specification!");
		} else {
			switch (v[0][1]) {
				case 'h':
					usage (appname, NULL);
					break;
				case 'o':
					switch (v[0][2]) {
						case 'l':
							if (c == 1) usage (appname, "-ol option requires argument");
							c--; v++;
							cfg_strdup (&cfg.outlib, v[0]);
							break;
						case 'h':
							if (c == 1) usage (appname, "-oh option requires argument");
							c--; v++;
							cfg_strdup (&cfg.outhdr, v[0]);
							break;
						case 'c':
							if (c == 1) usage (appname, "-oc option requires argument");
							c--; v++;
							cfg_strdup (&cfg.outsrc, v[0]);
							break;
						default:
							usage (appname, "unknown -o* option");
							break;
					}
					break;
				default:
					usage (appname, "unknown option");
			}
		}
		c--; v++;
	}
}

