/** \file
 * The Steel Panthers World at War savegame decomposition tool.
 *
 * Copyright (C) 2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "stdafx.h"

void
usage (char *app)
{
	printf ("Usage: %s DIR INDEX\n", app);
	printf ("Where: DIR     path to savegame dir\n");
	printf ("       INDEX   index of savegame to decompose\n");
	exit (0);
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
write_data (char *fn, void *data, DWORD size)
{
	int	fd;
	int	bw;

	fd = open (fn, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (fd == -1) error ("failed to open output file \"%s\" for writing!", fn);

	bw = write (fd, data, size);
	if (bw != (int)size)
		error ("failed to write savegame data to \"%s\"!", fn);

	close (fd);
}

int
main(int argc, char** argv)
{
	SPWAW_ERROR	rc;
	SPWAW_SAVEGAME	*game = NULL;
	char		fn[256];

	if (argc < 3) usage (argv[0]);

	memset (fn, 0, sizeof (fn));

	/* Initialize spwawlib */
	if ((rc = SPWAW_init (NULL, false)) != SPWERR_OK)
		error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));

	/* Load savegame data */
	if ((rc = SPWAW_savegame_load (argv[1], atoi(argv[2]), &game)) != SPWERR_OK)
		error ("failed to load savegame: %s", SPWAW_errstr (rc));

	/* Write savegame comment data */
	snprintf (fn, sizeof (fn) - 1, "comment.sect");
	write_data (fn, game->comment.data, game->comment.size);
	
	/* Write savegame sections data */
	//for (int i = 0; i < SPWAW_SECTION_COUNT; i++) {
	for (int i = 0; i < SPWW2_SECTION_COUNT; i++) {
		snprintf (fn, sizeof (fn) - 1, "section%2.2d.sect", game->sections[i].idx);
		write_data (fn, game->sections[i].data, game->sections[i].size);
	}

	/* Clean up */
	SPWAW_savegame_free (&game);
	SPWAW_shutdown ();

	return (0);
}
