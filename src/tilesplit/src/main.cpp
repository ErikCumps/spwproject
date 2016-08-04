/** \file
 * A simple tool to extract tiles from an image file.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "main.h"

static CONFIG	cfg;

static void
usage (char *app)
{
	printf ("Usage: %s filename width height [xpos ypos [subwidth subheight [subxpos subypos]]]\n", app);
	printf ("Where: filename   name of file to split\n");
	printf ("       width      width of tile\n");
	printf ("       height     height of tile\n");
	printf ("       xpos       optional x position of first tile\n");
	printf ("       ypos       optional y position of first tile\n");
	printf ("       subwidth   optional width of subtile\n");
	printf ("       subheight  optional height of subtile\n");
	printf ("       subxpos    optional x position of subtile\n");
	printf ("       subypos    optional y position of subtile\n");
	printf ("\n");
	exit (1);
}

static void
arguments (int argc, char **argv, CONFIG *cfg)
{
	int	i;

	if (argc < 4) usage (argv[0]);

	memset (cfg, 0, sizeof (CONFIG));
	cfg->sw = cfg->sh = cfg->sx = cfg->sy = -1;

	snprintf (cfg->src, sizeof (cfg->src) - 1, "%s", argv[1]);
	i = strlen (cfg->src) - 1;
	while ((i >= 0) && (cfg->src[i] != '.')) i--;
	if (i<0) usage (argv[0]);

	memcpy (&(cfg->stm), cfg->src, i);
	snprintf (cfg->ext, sizeof (cfg->ext) - 1, "%s", &(cfg->src[i+1]));

	cfg->w = atoi (argv[2]);
	cfg->h = atoi (argv[3]);

	if (argc > 4) {
		cfg->x = atoi (argv[4]);
		cfg->y = atoi (argv[5]);
	}

	if (argc > 6) {
		cfg->sw = atoi (argv[6]);
		cfg->sh = atoi (argv[7]);
		cfg->sx = 0;
		cfg->sy = 0;
	}

	if (argc > 8) {
		cfg->sx = atoi (argv[8]);
		cfg->sy = atoi (argv[9]);
	}

	printf ("# cfg.src = \"%s\"\n", cfg->src);
	printf ("# cfg.stm = \"%s\"\n", cfg->stm);
	printf ("# cfg.ext = \"%s\"\n", cfg->ext);
	printf ("# cfg.w   = %d\n", cfg->w);
	printf ("# cfg.h   = %d\n", cfg->h);
	printf ("# cfg.x   = %d\n", cfg->x);
	printf ("# cfg.y   = %d\n", cfg->y);
	printf ("# cfg.sw  = %d\n", cfg->sw);
	printf ("# cfg.sh  = %d\n", cfg->sh);
	printf ("# cfg.sx  = %d\n", cfg->sx);
	printf ("# cfg.sy  = %d\n", cfg->sy);
	printf ("\n");
}

static void
error (char *msg)
{
	fprintf (stderr, "*** Error: %s!\n", msg);
	exit (1);
}

static int
tilesplit (CONFIG *cfg)
{
	char	out[MAX_PATH+1];
	QPixmap	*src = NULL;
	QPixmap	*tile = NULL;
	QPixmap	*subtile = NULL;
	int	Sx, Sy, x, y;
	int	R, C;

	memset (out, 0, sizeof (out));

	src = new QPixmap (cfg->src, cfg->ext);
	if (!src) error ("failed to open input file");

	Sx = x = cfg->x; Sy = y = cfg->y;
	R = C = 0;
	while (y < src->height()) {
		while (x < src->width()) {
			memset (out, 0, sizeof (out));
			snprintf (out, sizeof (out) - 1, "%s_R%03u_C%03u.%s", cfg->stm, R, C, cfg->ext);
			printf ("TILE[R%03u, C%03u] (x=%04u, y=%04u)%s\n", R, C, x, y, out);

			tile = new QPixmap (src->copy (x, y, cfg->w, cfg->h));
			if (!tile) error ("failed to extract tile");

			if ((cfg->sw != -1) && (cfg->sh != -1)) {
				subtile = new QPixmap (tile->copy (cfg->sx, cfg->sy, cfg->sw, cfg->sh));
				if (!subtile) error ("failed to extract subtile");
				subtile->save (out, cfg->ext);
			} else {
				tile->save (out, cfg->ext);
			}

			x += cfg->w; C++;
		}
		x = Sx; C = 0;
		y += cfg->h; R++;
	}

	return (0);
}

int
main (int argc, char **argv)
{
	QApplication	*App = NULL;
	int		rc;

	App = new QApplication (argc, argv);

	arguments (argc, argv, &cfg);
	rc = tilesplit (&cfg);

	return (rc);
}
