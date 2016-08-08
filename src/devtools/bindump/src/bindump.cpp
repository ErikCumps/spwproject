/** \file
 * A simple tool to dump portions of a binary file.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "stdafx.h"

#define	BLKSZ	4096

void
usage (void)
{
	printf ("Usage: bintool <file> dump <start> <size> <outfile>\r\n");
	printf ("Where: file      data file to process\r\n");
	printf ("       start     start address of dump\r\n");
	printf ("       size      dump size in bytes\r\n");
	printf ("       outfile   output file\r\n");
	printf ("\r\n");
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
	fprintf (stderr, "*** Error: %s\r\n\r\n", buf);
	exit (10);
}

int
dump (char *file, int argc, char **argv)
{
	unsigned int	src, len;
	char		*outfile;
	int		fdi, fdo;
	unsigned int	left, todo, done;
	unsigned char	block[BLKSZ];

	printf ("DUMP \"%s\"\r\n", file);

	src = strtoul (argv[0], NULL, 0);
	len = strtoul (argv[1], NULL, 0);
	outfile = argv[2];

	printf ("(%lu :%lu) -> \"%s\"\r\n", src, len, outfile);

	fdi = open (file, O_RDONLY|O_BINARY);
	if (fdi == -1) error ("failed to open \"%s\" for reading!", file);

	fdo = open (outfile, O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (fdo == -1) error ("failed to open \"%s\" for writing!", outfile);

	if (lseek (fdi, src, SEEK_SET) == -1)
		error ("failed to seek to %lu in input file!", src);

	left = len;
	while (left) {
		todo = (left >=  BLKSZ) ? BLKSZ : left;
		done = read (fdi, block, todo);
		if (done != todo) error ("failed to read data from input file!");
		done = write (fdo, block, todo);
		if (done != todo) error ("failed to write data to output file!");
		left -= todo;
	}

	close (fdo); close (fdi);

	return (0);
}

int
main(int argc, char** argv)
{
	char	*file, *cmd;
	int	rc;

	if (argc < 6) usage ();

	file = argv[1];
	cmd  = argv[2];

	if (strcmp (cmd, "dump") != 0) usage ();
	rc = dump (file, argc - 2, &(argv[3]));
	return (rc);
}

