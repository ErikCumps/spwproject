/** \file
 * Simple tool to check if a file is newer than some other file.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdio.h>
#include <sys/stat.h>

int
main (int argc, char *argv[])
{
	struct stat	st1, st2;

	if (argc < 3) {
		printf ("Usage: %s file1 file2\n", argv[0]);
		printf ("Where: file1 file2   files to check\n");
		printf ("\n");
		printf ("Return codes:\n");
		printf ("\t0 - file1 is older than or has same age as file2\n");
		printf ("\t1 - file1 is newer than file2\n");
		printf ("\t2 - an error occurred\n");
		printf ("\n");
		return (2);
	}

	if (stat (argv[1], &st1) < 0) {
		perror ("Failed to stat file1");
		return (2);
	}

	if (stat (argv[2], &st2) < 0) return (1);

	if (st1.st_mtime > st2.st_mtime)
		return (1);
	else
		return (0);
}

