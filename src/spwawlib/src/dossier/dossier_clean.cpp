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

void
dossier_clean_turn (SPWAW_BTURN *t)
{
	if (!t) return;

	dossier_free_bturn_info	(&(t->info));
	t->snap->stab = NULL;
	SPWAW_snap_free (&(t->snap));
	safe_free (t);
}

void
dossier_clean_battle (SPWAW_BATTLE *b)
{
	if (!b) return;

	if (b->tlist) {
		for (DWORD j=0; j<b->tcnt; j++) {
			dossier_clean_turn (b->tlist[j]);
		}
		safe_free (b->tlist);
	}
	if (b->ra) safe_free (b->ra);
	safe_free (b);
}

SPWAW_ERROR
dossier_clean (SPWAW_DOSSIER *ptr)
{
	CNULLARG (ptr);

	if (ptr->blist) {
		for (DWORD i=0; i<ptr->bcnt; i++) {
			dossier_clean_battle (ptr->blist[i]);
		}
		safe_free (ptr->blist);
	}
	SPWOOB_free (&(ptr->oobdat));
	STRTAB_free ((STRTAB **)&(ptr->stab));

	clear_ptr (ptr);

	return (SPWERR_OK);
}
