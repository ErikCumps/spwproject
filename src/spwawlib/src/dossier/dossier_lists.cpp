/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
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
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i, j;
	SPWAW_BATTLE	*b = NULL;

	CNULLARG (dossier); CNULLARG (list);

	rc = SPWAW_savelist_new (list); ROE ("SPWAW_savelist_new()");

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
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i, j;
	SPWAW_BATTLE	*b = NULL;

	CNULLARG (dossier); CNULLARG (list);

	rc = SPWAW_snaplist_new (list); ROE ("SPWAW_snaplist_new()");

	if (dossier->blist) {
		for (i=0; i<dossier->bcnt; i++) {
			b = dossier->blist[i]; if (!b) continue;

			if (b->tlist) {
				for (j=0; j<b->tcnt; j++) {
					if (!(b->tlist[j])) continue;

					SPWAW_SNAPLIST_NODE *n = safe_malloc (SPWAW_SNAPLIST_NODE);
					COOM (n, "SPWAW_SNAPLIST_NODE element");

					snprintf (n->dir, sizeof (n->dir) - 1, "%s", b->tlist[j]->snap->src.path);
					snprintf (n->filename, sizeof (n->filename) - 1, "%s", b->tlist[j]->snap->src.file);
					n->filedate = b->tlist[j]->snap->src.date;

					snprintf (n->info.location, sizeof (n->info.location) - 1, "%s",
						b->tlist[j]->snap->raw.game.battle.location);
					snprintf (n->info.stamp, sizeof (n->info.stamp) - 1, "%s, turn %u",
						b->tlist[j]->snap->game.battle.strings.date,
						b->tlist[j]->snap->game.battle.data.turn);

					rc = SPWAW_snaplist_add (*list, n); ROE ("SPWAW_snaplist_add()");
				}
			}
		}
	}

	return (SPWERR_OK);
}
