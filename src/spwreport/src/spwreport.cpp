/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"
#include "utils.h"

void
usage (char *app)
{
	printf ("Generate SPWaW savegame reports (and an uncompressed snapshot).\n");
	printf ("\n");
	printf ("Usage: %s OOB DIR INDEX BASE [TYPE]\n", app);
	printf ("Where: OOB     path to oob dir\n");
	printf ("       DIR     path to savegame dir\n");
	printf ("       INDEX   index of savegame to generate report for\n");
	printf ("       BASE    basename for report files\n");
	printf ("       TYPE    optional type of report to generate (defaults to ALL reports)\n");
	printf ("\n");
	printf ("Supported report types:\n");
	printf ("    raw         raw savegame content reports\n");
	printf ("    elaborate   an elaborate report\n");
	printf ("    normal      a normal report\n");
	printf ("    narrative   a normal report in a narrative format\n");
	printf ("    table       a normal report in table format\n");
	printf ("    map         a map and unit positions dump\n");
	printf ("\n");
	printf ("For complete, short, narrative and table reports a set of 2 reports are\n");
	printf ("generated: a full report covering the full player1 and player2 forces and\n");
	printf ("a core report covering the player1 core force only.\n");
	printf ("\n");
	printf ("Generate an OOB dump in CSV format (spwoobdump_*.csv files).\n");
	printf ("\n");
	printf ("Usage: %s dump OOB\n", app);
	printf ("Where: OOB      path to oob dir\n");
	printf ("\n");
	exit (1);
}

static void
do_raw_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_RAW.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	raw_report (ptr, rf);
	fclose (rf);
}

static void
do_rawlink_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_RAWLINK.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	rawlink_report (ptr, rf);
	fclose (rf);
}

static void
do_full_complete_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_full_complete.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	complete_report (ptr, rf, false);
	fclose (rf);
}

static void
do_core_complete_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_core_complete.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	complete_report (ptr, rf, true);
	fclose (rf);
}

static void
do_full_short_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_full_short.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	short_report (ptr, rf, false);
	fclose (rf);
}

static void
do_core_short_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_core_short.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	short_report (ptr, rf, true);
	fclose (rf);
}

static void
do_full_narrative_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_full_narrative.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	narrative_report (ptr, rf, false);
	fclose (rf);
}

static void
do_core_narrative_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_core_narrative.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	narrative_report (ptr, rf, true);
	fclose (rf);
}

static void
do_full_table_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_full_table.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	table_report (ptr, rf, false);
	fclose (rf);
}

static void
do_core_table_report (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_core_table.txt", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	table_report (ptr, rf, true);
	fclose (rf);
}

static void
do_map_dump (SPWAW_SNAPSHOT *ptr, char *rbfn)
{
	char		rfn[256];
	FILE		*rf = NULL;

	memset (rfn, 0, sizeof (rfn));
	snprintf (rfn, sizeof (rfn) - 1, "%s_positions.dump", rbfn);
	rf = fopen (rfn, "w");
	if (rf == NULL) {
		error ("failed to open report file \"%s\" for writing!", rfn);
		return;
	}

	map_dump (ptr, rf);
	fclose (rf);
}

int
main (int argc, char** argv)
{
	SPWAW_ERROR	rc;
	SPWAW_SNAPSHOT	*snap;

	if (argc < 3) usage (argv[0]);

	if (argc == 3) {
		// Generate OOB dump

		SPWOOB	*oob;

		if ((rc = SPWAW_init (argv[2], false)) != SPWERR_OK) {
			error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));
		}

		if ((rc = SPWAW_SPWOOB(&oob)) != SPWERR_OK) {
			error ("failed to obtain OOB data: %s", SPWAW_errstr (rc));
		}

		if ((rc = SPWAW_oob_dump (oob, "spwoobdump")) != SPWERR_OK) {
			error ("failed to generate oob dump: %s", SPWAW_errstr (rc));
		}

	} else {
		char	savename[MAX_PATH+1];
		char	rprtbase[MAX_PATH+1];

		if (argc < 5) usage (argv[0]);

		// Generate report

		if ((rc = SPWAW_init (argv[1], true)) != SPWERR_OK) {
			error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));
		}

		if ((rc = SPWAW_snap_make (argv[2], atoi(argv[3]), &snap)) != SPWERR_OK) {
			error ("failed to create snapshot for \"%s:%s\": %s", argv[2], argv[3], SPWAW_errstr (rc));
		}

		memset (rprtbase, 0, sizeof (rprtbase));
		snprintf (rprtbase, sizeof (rprtbase) - 1, "%s", argv[4]);

		bool	raw	  = false;
		bool	elaborate = false;
		bool	normal	  = false;
		bool	narrative = false;
		bool	table	  = false;
		bool	map	  = false;

		if (argc > 5) {
			if (strcmp (argv[5], "raw") == 0)	raw	  = true;
			if (strcmp (argv[5], "elaborate") == 0)	elaborate = true;
			if (strcmp (argv[5], "normal") == 0)	normal	  = true;
			if (strcmp (argv[5], "narrative") == 0)	narrative = true;
			if (strcmp (argv[5], "table") == 0)	table	  = true;
			if (strcmp (argv[5], "map") == 0)	map	  = true;
		} else {
			raw = elaborate = normal = narrative = table = map = true;
		}

		if (raw) {
			do_raw_report		(snap, rprtbase);
			do_rawlink_report	(snap, rprtbase);
		}
		if (elaborate) {
			do_full_complete_report	(snap, rprtbase);
			do_core_complete_report	(snap, rprtbase);
		}
		if (normal) {
			do_full_short_report	(snap, rprtbase);
			do_core_short_report	(snap, rprtbase);
		}
		if (narrative) {
			do_full_narrative_report(snap, rprtbase);
			do_core_narrative_report(snap, rprtbase);
		}
		if (table) {
			do_full_table_report	(snap, rprtbase);
			do_core_table_report	(snap, rprtbase);
		}
		if (map) {
			do_map_dump		(snap, rprtbase);
		}

		memset (savename, 0, sizeof (savename));
		snprintf (savename, sizeof (savename) - 1, "snapshot_%lu.snap", atoi(argv[3]));
		if ((rc = SPWAW_snap_save (&snap, savename, false)) != SPWERR_OK) {
			error ("failed to save snapshot as \"%s\": %s", savename, SPWAW_errstr (rc));
		}

		if ((rc = SPWAW_snap_free (&snap)) != SPWERR_OK) {
			error ("failed to free snapshot: %s", SPWAW_errstr (rc));
		}
	}

	SPWAW_shutdown ();

	return (0);
}

