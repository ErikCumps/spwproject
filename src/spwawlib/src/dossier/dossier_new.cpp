#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "spwoob/spwoob.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_new (SPWAW_DOSSIER **ptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_DOSSIER	*p = NULL;

	CNULLARG (ptr);
	*ptr = NULL;

	p = safe_malloc (SPWAW_DOSSIER); COOMGOTO (p, "SPWAW_DOSSIER", handle_error);

	rc = SPWOOB_new (&(p->oobdat));
	ERRORGOTO ("spwoob_new()", handle_error);

	rc = STRTAB_new ((STRTAB **)&(p->stab));
	ERRORGOTO ("STRTAB_new()", handle_error);

	*ptr = p;
	return (SPWERR_OK);

handle_error:
	if (p) {
		if (p->stab) STRTAB_free ((STRTAB **)&(p->stab));
		if (p->oobdat) SPWOOB_free (&(p->oobdat));
		free (p);
	}
	return (rc);
}
