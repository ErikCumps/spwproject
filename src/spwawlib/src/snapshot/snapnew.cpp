/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
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

	p->oobdat = oobptr;
	p->freeoobdat = false;

	if (!stab) {
		rc = STRTAB_new (&stab);
		ERRORGOTO ("STRTAB_new()", handle_error);
	}
	p->stab = stab;

	p->type = SPWAW_UNKNOWN_BATTLE;

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
