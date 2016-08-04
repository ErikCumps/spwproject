/** \file
 * A simple tool to convert an image file to XPM.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "main.h"

void
usage (char *app)
{
	printf ("Usage: %s filename [width]\n", app);
	printf ("Where: filename   name of file to convert\n");
	printf ("       width      scale the converted image to width\n");
	printf ("\n");
	exit (1);
}

int
main (int argc, char **argv)
{
	char	src[MAX_PATH+1], ext[MAX_PATH+1];
	QString	dst;
	int	i, w = 0;

	if (argc < 2) usage (argv[0]);

	memset (src, 0, sizeof (src));
	memset (ext, 0, sizeof (ext));

	snprintf (src, sizeof (src) - 1, "%s", argv[1]);
	i = strlen (src) - 1;
	while ((i >= 0) && (src[i] != '.')) i--;
	if (i<0) usage (argv[0]);

	src[i] = '\0'; i++;
	snprintf (ext, sizeof (ext) - 1, "%s", &(src[i]));

	if (strncmp (ext, "xpm", 4) == 0) {
		dst.sprintf ("new_%s.xpm", src);
	} else {
		dst.sprintf ("%s.xpm", src);
	}

	if (argc > 2) { w = atoi (argv[2]); }

	QApplication App (argc, argv);
	QPixmap srcpm (src, ext);

	if (w) srcpm = srcpm.scaledToWidth (w, Qt::FastTransformation);

	srcpm.save (dst, "xpm");

	return (0);
}
