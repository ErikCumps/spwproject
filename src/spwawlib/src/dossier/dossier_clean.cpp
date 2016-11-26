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
#include "common/internal.h"

SPWAW_ERROR
dossier_clean (SPWAW_DOSSIER *ptr)
{
	DWORD	i, j;

	CNULLARG (ptr);

	if (ptr->blist) {
		for (i=0; i<ptr->bcnt; i++) {
			SPWAW_BATTLE *b = ptr->blist[i]; if (!b) continue;

			if (b->tlist) {
				for (j=0; j<b->tcnt; j++) {
					if (!(b->tlist[j])) continue;

					dossier_free_bturn_info	(&(b->tlist[j]->info));
					b->tlist[j]->snap->stab = NULL;
					SPWAW_snap_free (&(b->tlist[j]->snap));
					free (b->tlist[j]);
				}
				free (b->tlist);
			}
			if (b->ra) safe_free (b->ra);
			free (b);
		}
		safe_free (ptr->blist);
	}
	SPWOOB_free (&(ptr->oobdat));
	STRTAB_free ((STRTAB **)&(ptr->stab));

	clear_ptr (ptr);

	return (SPWERR_OK);
}
