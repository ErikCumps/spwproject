/** \file
 * The SPWaW Library - main API implementation.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "utils/log.h"
#include "common/internal.h"

bool spwaw_initialized = false;

static const char *errstr[SPWERR_LAST_CODE+1] = {
	"SPWERR_OK: no error",
	"SPWERR_FAILED: failure",
	"SPWERR_NULLARG: unexpected NULL argument",
	"SPWERR_OUTOFMEM: out of memory",
	"SPWERR_NOTINIT: spwawlib not yet initialized",
	"SPWERR_NOOOBFILES: no OOB data files found",
	"SPWERR_BADOOB: bad OOB data",
	"SPWERR_BADSAVEGAME: bad save game",
	"SPWERR_BADSAVEDATA: bad save game data",
	"SPWERR_BADSTABDATA: bad string table data",
	"SPWERR_BADSTABREFCNT: bad string table reference count",
	"SPWERR_BADSBUFDATA: bad string buffer data",
	"SPWERR_FOFAILED: file open failed",
	"SPWERR_FRFAILED: file read failed",
	"SPWERR_FWFAILED: file write failed",
	"SPWERR_CORRUPT: file corruption detected",
	"SPWERR_BADDATE: invalid date",
	"SPWERR_BADSTAMP: invalid timestamp",
	"SPWERR_BADPERIOD: invalid period",
	"SPWERR_NOMATCH_OOBDATA: OOB data files do not match",
	"SPWERR_NOMATCH_OOB: player OOB ID does not match",
	"SPWERR_NOMATCH_CORECNT: formation or unit core count does not match",
	"SPWERR_DUPTURN: duplicate battle turn",
};

SPWAWLIB_API const char *
SPWAW_errstr (SPWAW_ERROR e)
{
	return (errstr[e]);
}

static SPWAW_ERROR
spwaw_recfg (const char *oobdir, bool withUD)
{
	return cfg_set (oobdir, withUD);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_init (const char *oobdir, bool withUD)
{
	SPWAW_ERROR	rc;

	log_init ("SPWAWLIB.log");

	if (spwaw_initialized) FAILGOTO (SPWERR_FAILED, "library already initialized", handle_error);

	rc = spwaw_recfg (oobdir, withUD);
	ERRORGOTO ("spwaw_recfg()", handle_error);

	spwaw_initialized = true;
	return (SPWERR_OK);

handle_error:
	log_shutdown ();
	return (rc);
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_recfg (const char *oobdir, bool withUD)
{
	CSPWINIT;

	return (spwaw_recfg (oobdir, withUD));
}

SPWAWLIB_API void
SPWAW_shutdown (void)
{
	if (!spwaw_initialized) return;

	cfg_free ();
	spwaw_initialized = false;

	log_shutdown ();
}

SPWAWLIB_API SPWAW_ERROR
SPWAW_SPWOOB (SPWOOB **oob)
{
	CSPWINIT;
	CNULLARG (oob);
	*oob = NULL;

	if (!cfg.oobptr) RWE (SPWERR_NOOOBFILES, "no OOB directory configured");

	*oob = cfg.oobptr;
	return (SPWERR_OK);
}
