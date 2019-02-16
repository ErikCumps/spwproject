/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#include "spwreport.h"
#include "utils.h"

void
usage (char *app)
{
	printf ("Welcome to spwreport.exe, the multi-purpose spwawlib tool:\n");
	printf ("\n");

	printf ("*** Generate an OOB dump in CSV format (spwoobdump_*.csv files).\n");
	printf ("\n");
	printf ("    Usage: %s oobdump GAME OOB\n", app);
	printf ("    Where: GAME    game type\n");
	printf ("           OOB     path to oob dir\n");
	printf ("\n");

	printf ("*** Generate savegame reports:\n");
	printf ("\n");
	printf ("    Usage: %s savereport GAME OOB DIR INDEX BASE [TYPE]\n", app);
	printf ("    Where: GAME    game type\n");
	printf ("           OOB     path to oob dir\n");
	printf ("           DIR     path to savegame dir\n");
	printf ("           INDEX   index of savegame to generate report for\n");
	printf ("           BASE    basename for report files\n");
	printf ("           TYPE    optional type of report to generate (defaults to ALL reports)\n");
	printf ("\n");

	printf ("*** Generate snapshot reports:\n");
	printf ("\n");
	printf ("    Usage: %s snapreport SNAP BASE [TYPE]\n", app);
	printf ("    Where: SNAP    filename of snapshot to generate report for\n");
	printf ("           BASE    basename for report files\n");
	printf ("           TYPE    optional type of report to generate (defaults to ALL reports)\n");
	printf ("\n");

	printf ("Supported game types:\n");
	printf ("    SPWAW       SPWaW\n");
	printf ("    WINSPWW2    winSPWW2\n");
	printf ("\n");

	printf ("Supported report types:\n");
	printf ("    raw         raw savegame content reports (*_RAW.txt and *_RAWLINK.txt)\n");
	printf ("    elaborate   an elaborate report (*_complete.txt)\n");
	printf ("    normal      a normal report (*_short.txt)\n");
	printf ("    narrative   a normal report in a narrative format (*_narrative.txt)\n");
	printf ("    table       a normal report in table format (*_table.txt)\n");
	printf ("    map         a map and unit positions dump (*_positions.dump)\n");
	printf ("\n");

	printf ("Two reports are generate for elaborate, normal, narrative and table reports:\n");
	printf ("+ a full report covering the full player1 and player2 forces\n");
	printf ("+ a core report covering the player1 core force only.\n");
	printf ("\n");
	exit (1);
}

void
generate_oob_dump(int /*argc*/, char** argv)
{
	SPWAW_GAME_TYPE	gametype;
	SPWAW_ERROR	rc;
	SPWOOB		*oob;

	gametype = SPWAW_str2gametype (argv[2]);

	if ((rc = SPWAW_init (gametype, argv[3], false)) != SPWERR_OK) {
		error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));
	}

	if ((rc = SPWAW_SPWOOB(&oob)) != SPWERR_OK) {
		error ("failed to obtain OOB data: %s", SPWAW_errstr (rc));
	}

	if ((rc = SPWAW_oob_dump (oob, "spwoobdump")) != SPWERR_OK) {
		error ("failed to generate oob dump: %s", SPWAW_errstr (rc));
	}
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

void
generate_reports(int argc, char** argv, char *base, SPWAW_SNAPSHOT *snap)
{
	char	rprtbase[MAX_PATH+1];

	memset (rprtbase, 0, sizeof (rprtbase));
	snprintf (rprtbase, sizeof (rprtbase) - 1, "%s", base);

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
}

void
generate_savegame_report(int argc, char** argv)
{
	SPWAW_GAME_TYPE	gametype;
	SPWAW_ERROR	rc;
	SPWAW_SNAPSHOT	*snap;
	char		savename[MAX_PATH+1];

	gametype = SPWAW_str2gametype (argv[2]);

	if ((rc = SPWAW_init (gametype, argv[3], true)) != SPWERR_OK) {
		error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));
	}

	if ((rc = SPWAW_snap_make (gametype, argv[4], atoi(argv[5]), &snap)) != SPWERR_OK) {
		error ("failed to create snapshot for \"%s:%s\": %s", argv[4], argv[5], SPWAW_errstr (rc));
	}

	generate_reports (argc, argv, argv[6], snap);

	memset (savename, 0, sizeof (savename));
	snprintf (savename, sizeof (savename) - 1, "snapshot_%s.snap", argv[5]);
	if ((rc = SPWAW_snap_save (&snap, savename, false)) != SPWERR_OK) {
		error ("failed to save snapshot as \"%s\": %s", savename, SPWAW_errstr (rc));
	}

	if ((rc = SPWAW_snap_free (&snap)) != SPWERR_OK) {
		error ("failed to free snapshot: %s", SPWAW_errstr (rc));
	}
}

void
generate_snapshot_report(int argc, char** argv)
{
	SPWAW_ERROR	rc;
	SPWAW_SNAPSHOT	*snap;

	if ((rc = SPWAW_init (SPWAW_GAME_TYPE_SPWAW, NULL, true)) != SPWERR_OK) {
		error ("failed to initialize spwawlib: %s", SPWAW_errstr (rc));
	}

	if ((rc = SPWAW_snap_load (argv[2], &snap)) != SPWERR_OK) {
		error ("failed to load snapshot \"%s\": %s", argv[2], SPWAW_errstr (rc));
	}

	generate_reports (argc, argv, argv[3], snap);

	if ((rc = SPWAW_snap_free (&snap)) != SPWERR_OK) {
		error ("failed to free snapshot: %s", SPWAW_errstr (rc));
	}
}

int
main (int argc, char** argv)
{
	if (argc < 2)
	{
		usage (argv[0]);
	}
	else if ((strcmp (argv[1], "oobdump") == 0) && (argc == 4))
	{
		generate_oob_dump (argc, argv);
	}
	else if ((strcmp (argv[1], "savereport") == 0) && (argc >= 7))
	{
		generate_savegame_report (argc, argv);
	}
	else if ((strcmp (argv[1], "snapreport") == 0) && (argc >= 4))
	{
		generate_snapshot_report (argc, argv);
	}
	else
	{
		usage (argv[0]);
	}

	SPWAW_shutdown ();

	return (0);
}

