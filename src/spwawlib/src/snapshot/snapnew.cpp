/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "spwoob/spwoob.h"
#include "common/internal.h"

SPWAW_ERROR
snapnew (SPWAW_SNAPSHOT **ptr, SPWOOB *oobptr, STRTAB *stabptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_SNAPSHOT	*p = NULL;
	STRTAB		*stab = stabptr;

	CNULLARG (ptr);
	*ptr = NULL;

	p = safe_malloc (SPWAW_SNAPSHOT); COOMGOTO (p, "SPWAW_SNAPSHOT", handle_error);

	rc = SPWOOB_new (&(p->oobdat));
	ERRORGOTO ("SPWOOB_new()", handle_error);

	if (oobptr) {
		rc = SPWOOB_copy (p->oobdat, oobptr);
		ERRORGOTO ("SPWOOB_copy()", handle_error);
	}

	if (!stab) {
		rc = STRTAB_new (&stab);
		ERRORGOTO ("STRTAB_new()", handle_error);
	}
	p->stab = stab;

	*ptr = p;
	return (SPWERR_OK);

handle_error:
	if (p) {
		if (p->stab) STRTAB_free ((STRTAB **)&(p->stab));
		SPWOOB_free (&(p->oobdat));
		free (p);
	}
	return (rc);
}
