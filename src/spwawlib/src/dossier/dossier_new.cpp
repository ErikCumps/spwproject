/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "spwoob/spwoob.h"
#include "spwoob/spwoob_list.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_new (SPWAW_DOSSIER **ptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_DOSSIER	*p = NULL;

	CNULLARG (ptr);
	*ptr = NULL;

	p = safe_malloc (SPWAW_DOSSIER); COOMGOTO (p, "SPWAW_DOSSIER", handle_error);

	rc = SPWOOB_LIST_new (&(p->oobdata));
	ERRORGOTO ("SPWOOB_LIST_new()", handle_error);

	rc = STRTAB_new ((STRTAB **)&(p->stab));
	ERRORGOTO ("STRTAB_new()", handle_error);

	*ptr = p;
	return (SPWERR_OK);

handle_error:
	if (p) {
		if (p->stab) STRTAB_free ((STRTAB **)&(p->stab));
		if (p->oobdata) SPWOOB_LIST_free (&(p->oobdata));
		free (p);
	}
	return (rc);
}
