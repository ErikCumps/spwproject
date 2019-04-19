/** \file
 * The SPWaW Library - snapshot API implementation.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_snapshot.h>
#include "gamefile/game.h"
#include "snapshot/snapshot.h"
#include "common/internal.h"
#include "utils/filecheck.h"

/*! Create snapshot from SPWAW savegame */
SPWAWLIB_API SPWAW_ERROR
SPWAW_snap_make (SPWAW_GAME_TYPE gametype, const char *dir, int id, SPWAW_SNAPSHOT **snap)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_SNAPSHOT	*ptr = NULL;
	STRTAB		*stab = NULL;
	GAMEDATA	*data = NULL;
	GAMEINFO	info;

	CSPWINIT;
	CNULLARG (dir); CNULLARG (snap);

	/* Create snapshot structure and string table */
	rc = snapnew (&ptr, cfg.oobptr, NULL);
	ERRORGOTO ("snapnew()", handle_error);
	stab = (STRTAB *)ptr->stab;

	/* Open savegame, load and decompress data */
	data = game_load_full (gametype, dir, id, &info);
	if (!data) FAILGOTO (SPWERR_BADSAVEGAME, "game_load_full()", handle_error);

	/* Record source game data */
	ptr->src.path = STRTAB_add (stab, info.path);
	ptr->src.file = STRTAB_add (stab, info.file);
	ptr->src.date = info.date;

	/* Extract and interprete data from savegame data */
	rc = load_from_game (data, ptr);
	ERRORGOTO ("load_from_game()", handle_error);

	rc = snapint (ptr);
	ERRORGOTO ("snapint()", handle_error);

	/* Set snapshot battle type */
	ptr->type = data->type;

	/* Cleanup and return */
	gamedata_free (&data);

	*snap = ptr;
	return (SPWERR_OK);

handle_error:
	if (data) gamedata_free (&data);
	SPWAW_snap_free (&ptr);
	return (rc);
}

/*! Obtain snapshot info from file */
SPWAWLIB_API SPWAW_ERROR
SPWAW_snap_info (const char *file, SPWAW_SNAPSHOT_INFO *info)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	int		fd;

	CSPWINIT;
	CNULLARG (file); CNULLARG (info);
	clear_ptr (info);

	/* Open file and load snapshot headers and string table */
	fd = open (file, O_RDONLY|O_BINARY);
	if (fd < 0) RWE (SPWERR_FRFAILED, "failed to open snapshot file");

	/* Verify file integrity */
	rc = fcheck_verify (fd);
	ROE ("fcheck_verify()");

	/* Load snapshot info */
	rc = snaploadinfo (fd, info);
	ROE ("snaploadinfo()");

	/* Cleanup and return */
	close (fd);

	return (SPWERR_OK);
}

/*! Load snapshot from file */
SPWAWLIB_API SPWAW_ERROR
SPWAW_snap_load	(const char *file, SPWAW_SNAPSHOT **snap)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_SNAPSHOT	*ptr = NULL;
	int		fd = -1;

	CSPWINIT;
	CNULLARG (file); CNULLARG (snap);

	/* Create snapshot structure and string table */
	rc = snapnew (&ptr, NULL, NULL);
	ERRORGOTO ("snapnew()", handle_error);

	/* Open file */
	fd = open (file, O_RDONLY|O_BINARY);
	if (fd < 0) FAILGOTO (SPWERR_FRFAILED, "failed to open snapshot file for reading", handle_error);

	/* Verify file integrity */
	rc = fcheck_verify (fd);
	ERRORGOTO ("fcheck_verify()", handle_error);

	/* Load snapshot data */
	rc = snapload (fd, ptr, NULL);
	ERRORGOTO ("snapload()", handle_error);

	/* Interprete data in snapshot */
	rc = snapint (ptr);
	ERRORGOTO ("snapint()", handle_error);

	/* Cleanup and return */
	close (fd); fd = -1;

	*snap = ptr;
	return (SPWERR_OK);

handle_error:
	if (fd >= 0) close (fd);
	SPWAW_snap_free (&ptr);
	return (rc);
}

/*! Save snapshot to file */
SPWAWLIB_API SPWAW_ERROR
SPWAW_snap_save	(SPWAW_SNAPSHOT **snap, const char *file, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	char		tf[MAX_PATH+1];
	char		bf[MAX_PATH+1];
	int		fd;

	CSPWINIT;
	CNULLARG (snap); CNULLARG (*snap); CNULLARG (file);

	memset (tf, 0, sizeof (tf));
	snprintf (tf, sizeof (tf) - 1, "%s.save.", file);

	memset (bf, 0, sizeof (bf));
	snprintf (bf, sizeof (bf) - 1, "%s.backup", file);

	/* Open .save. file and save snapshot data */
	fd = open (tf, O_RDWR|O_BINARY|O_CREAT|O_TRUNC, 0666);
	if (fd < 0) FAILGOTO (SPWERR_FOFAILED, "snapshot .save. file create", handle_error);

	rc = snapsave (*snap, fd, true, true, compress);
	ERRORGOTO ("snapsave()", handle_error);

	rc = fcheck_make (fd);
	ERRORGOTO ("fcheck_make()", handle_error);

	close (fd);

	/* Safely replace original snapshot file with .save. file */
	unlink (bf);
	if ((rename (file, bf) != 0) && (errno != ENOENT)) {
		FAILGOTO (SPWERR_FWFAILED, "safe snapshot backup", handle_error);
	}
	if (rename (tf, file) != 0) FAILGOTO (SPWERR_FWFAILED, "safe snapshot replace", handle_error);
	unlink (bf);

	return (SPWERR_OK);

handle_error:
	if (fd >= 0) close (fd);
	return (rc);
}

/*! Destroy snapshot */
SPWAWLIB_API SPWAW_ERROR
SPWAW_snap_free (SPWAW_SNAPSHOT **snap)
{
	SPWAW_SNAPSHOT	*p;

	CSPWINIT;
	CNULLARG (snap); CNULLARG (*snap);
	p = *snap; *snap = NULL;

	if (p) {
		snapclean (p, NULL);
		free (p);
	}

	return (SPWERR_OK);
}

/*! Create copy of snapshot */
SPWAWLIB_API SPWAW_ERROR
SPWAW_snap_copy (SPWAW_SNAPSHOT **src, SPWAW_SNAPSHOT **dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CSPWINIT;
	CNULLARG (src); CNULLARG (*src); CNULLARG (dst);

	/* Cleanup destination snapshot if any */
	if (*dst) {
		SPWAW_snap_free (dst);
		*dst = NULL;
	}

	/* Create snapshot structure and string table */
	rc = snapnew (dst, NULL, NULL);
	ERRORGOTO ("snapnew()", handle_error);

	/* Copy snapshot */
	rc = snapcopy (*src, *dst);
	ROE ("snapcopy()");

	return (SPWERR_OK);

handle_error:
	if (*dst) {
		SPWAW_snap_free (dst);
		*dst = NULL;
	}
	return (rc);
}
