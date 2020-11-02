/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_savelist (SPWAW_DOSSIER *dossier, SPWAW_SAVELIST **list)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWAW_SAVELIST_TARGET	target;
	DWORD			i, j;
	SPWAW_BATTLE		*b = NULL;

	CNULLARG (dossier); CNULLARG (list);

	target.gametype	= dossier->gametype;
	target.type	= dossier->type;
	target.oobdir	= dossier->oobdir;

	rc = SPWAW_savelist_new (&target, list); ROE ("SPWAW_savelist_new()");

	if (dossier->blist) {
		for (i=0; i<dossier->bcnt; i++) {
			b = dossier->blist[i]; if (!b) continue;

			if (b->tlist) {
				for (j=0; j<b->tcnt; j++) {
					if (!(b->tlist[j])) continue;

					rc = SPWAW_savelist_add (*list, b->tlist[j]->snap); ROE ("SPWAW_savelist_add()");
				}
			}
		}
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_snaplist (SPWAW_DOSSIER *dossier, SPWAW_SNAPLIST **list)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	SPWAW_SNAPLIST_TARGET	target;
	DWORD			i, j;
	SPWAW_BATTLE		*b = NULL;

	CNULLARG (dossier); CNULLARG (list);

	target.gametype = dossier->gametype;
	target.type = dossier->type;

	rc = SPWAW_snaplist_new (&target, list); ROE ("SPWAW_snaplist_new()");

	if (dossier->blist) {
		for (i=0; i<dossier->bcnt; i++) {
			b = dossier->blist[i]; if (!b) continue;

			if (b->tlist) {
				for (j=0; j<b->tcnt; j++) {
					if (!(b->tlist[j])) continue;

					rc = SPWAW_snaplist_add (*list, b->tlist[j]->snap); ROE ("SPWAW_snaplist_add()");
				}
			}
		}
	}

	return (SPWERR_OK);
}
