/** \file
 * The Steel Panthers World at War savegame reconstruction tool.
 *
 * Copyright (C) 2016-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "stdafx.h"

void
usage (char *app)
{
	printf ("Welcome to spwreconstruct.exe, the savegame reconstruction tool:\n");
	printf ("\n");

	printf ("Usage: %s GAME DIR INDEX\n", app);
	printf ("    or %s GAME DIR NAME\n", app);
	printf ("Where: GAME    game type\n");
	printf ("       DIR     path to savegame dir\n");
	printf ("       INDEX   numerical index of savegame to decompose\n");
	printf ("       NAME    basename of savegame to decompose\n");
	printf ("\n");

	printf ("Supported game types:\n");
	printf ("    SPWAW       SPWaW\n");
	printf ("    WINSPWW2    winSPWW2\n");
	printf ("\n");

	printf ("The savegame is reconstructed, from separate sections, from the current directory.\n");
	printf ("\n");

	exit (1);
}

void
error (char *fmt, ...)
{
	va_list	AP;
	char	buf[1024];

	memset (buf, 0, sizeof (buf));
	va_start (AP, fmt);
	vsnprintf (buf, sizeof (buf) - 1, fmt, AP);
	va_end (AP);
	fprintf (stderr, "*** Error: %s\n\n", buf);
	exit (10);
}

void
warn (char *fmt, ...)
{
	va_list	AP;
	char	buf[1024];

	memset (buf, 0, sizeof (buf));
	va_start (AP, fmt);
	vsnprintf (buf, sizeof (buf) - 1, fmt, AP);
	va_end (AP);
	fprintf (stderr, "+++ Warning: %s\n\n", buf);
}

void
read_data (char *fn, void **data, DWORD *size)
{
	int		fd;
	struct stat	st;
	int		br;

	if (*data || *size) error ("INTERNAL ERROR!");
	*data = NULL;
	*size = 0;

	fd = open (fn, O_RDONLY|O_BINARY, 0666);
	if (fd == -1) {
		warn ("failed to open input file \"%s\" for reading", fn);
		return;
	}

	if (fstat (fd, &st) == -1) error ("failed to stat() input file \"%s\"!", fn);

	*size = (DWORD)st.st_size;
	*data = malloc (*size);
	if (!*data) error ("out of memory allocating savegame data buffer for \"%s\"!", fn);

	br = read (fd, *data, *size);
	if (br != (int)*size) error ("failed to read savegame data from \"%s\"!", fn);

	close (fd);
}

int
main (int argc, char** argv)
{
	SPWAW_GAME_TYPE			gametype;
	SPWAW_OOBCFG			oobcfg[1];
	SPWAW_SAVE_TYPE			savetype;
	SPWAW_SAVEGAME_DESCRIPTOR	sgd;
	SPWAW_ERROR			rc;
	SPWAW_SAVEGAME			*game = NULL;
	char				fn[256];

	if (argc < 4) usage (argv[0]);

	/* Determine game and save types */
	gametype = SPWAW_str2gametype (argv[1]);
	if ((argv[3][0] >= '0') && (argv[3][0] <= '9')) {
		savetype = SPWAW_SAVE_TYPE_REGULAR;
	} else {
		savetype = (gametype == SPWAW_GAME_TYPE_SPWAW) ? SPWAW_SAVE_TYPE_MEGACAM : SPWAW_SAVE_TYPE_REGULAR;
	}

	memset (fn, 0, sizeof (fn));

	/* Initialize spwawlib */
	oobcfg[0].gametype = gametype;
	oobcfg[0].oobdir   = NULL;
	if ((rc = SPWAW_init (oobcfg, 1, false)) != SPWERR_OK)
		error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));

	/* Create new, empty, savegame data structure */
	if ((rc = SPWAW_savegame_new (gametype, savetype, &game)) != SPWERR_OK)
		error ("failed to create new, empty, savegame data structure: %s", SPWAW_errstr (rc));

	/* Read savegame metadata */
	snprintf (fn, sizeof (fn) - 1, "metadata.sect");
	read_data (fn, &(game->metadata.data), &(game->metadata.size));
	
	/* Read savegame sections data */
	for (int i = 0; i < game->seccnt; i++) {
		snprintf (fn, sizeof (fn) - 1, "section%2.2d.sect", game->seclst[i].idx);
		read_data (fn, &(game->seclst[i].data), &(game->seclst[i].size));
	}

	/* Write savegame data */
	if ((argv[3][0] >= '0') && (argv[3][0] <= '9')) {
		rc = SPWAW_savegame_descriptor_init (sgd, gametype, savetype, oobcfg[0].oobdir, argv[2], atoi(argv[3]));
	} else {
		rc = SPWAW_savegame_descriptor_init (sgd, gametype, savetype, oobcfg[0].oobdir, argv[2], argv[3]);
	}
	if (rc != SPWERR_OK) {
		error ("failed to prepare savegame descriptor for \"%s:%s\": %s", argv[2], argv[3], SPWAW_errstr (rc));
	}

	if ((rc = SPWAW_savegame_save (&game, &sgd)) != SPWERR_OK) {
		error ("failed to write savegame for \"%s:%s\": %s", argv[2], argv[3], SPWAW_errstr (rc));
	}

	/* Clean up */
	SPWAW_savegame_descriptor_clear (sgd);
	SPWAW_savegame_free (&game);
	SPWAW_shutdown ();

	return (0);
}

