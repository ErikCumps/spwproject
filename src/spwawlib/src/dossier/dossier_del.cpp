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
#include "spwoob/spwoob_list.h"
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "common/internal.h"

static void
unlink_battle (SPWAW_BATTLE *p)
{
	if (!p) return;

	if (p->prev) p->prev->next = p->next;
	if (p->next) p->next->prev = p->prev;
}

static void
dossier_del_battle (SPWAW_DOSSIER *ptr, SPWAW_BATTLE *b, STRTAB *stab)
{
	USHORT		i, j;
	SPWAW_BATTLE	*bb = NULL, *ba = NULL;

	unlink_battle (b);

	bb = b->prev;
	ba = b->next;

	SPWOOB_LIST_del (ptr->oobdata, b->oobdat);
	STRTAB_del (stab, b->location);
	STRTAB_del (stab, b->miss_p1);
	STRTAB_del (stab, b->miss_p2);
	if (b->ra) safe_free (b->ra);
	free (b);

	for (i=0; i<ptr->bcnt; i++) {
		if (ptr->blist[i] != b) continue;

		for(j=i+1; j<ptr->bcnt; j++) ptr->blist[j-1] = ptr->blist[j];
		ptr->blist[ptr->bcnt-1] = NULL;
		ptr->bcnt--;
		break;
	}

	// TODO: investigate usefulnes of auto-cleanup
	if (ptr->bcnt == 0) {
		safe_free (ptr->blist);
		ptr->blen = 0;
	} else {
		ptr->bfirst = ptr->blist[0];
		ptr->blast  = ptr->blist[ptr->bcnt-1];
		dossier_update_battle_rainfo (bb, ba);
	}
}

static void
unlink_bturn (SPWAW_BTURN *p)
{
	if (!p) return;

	if (p->prev) p->prev->next = p->next;
	if (p->next) p->next->prev = p->prev;
}

static void
battle_del_battle_turn (SPWAW_BATTLE *ptr, SPWAW_BTURN *bturn, STRTAB *stab)
{
	USHORT	i, j;

	if (!battle_contains_bturn (ptr, bturn)) return;

	unlink_bturn (bturn);

	for (i=0; i<ptr->tcnt; i++) {
		if (ptr->tlist[i] != bturn) continue;

		dossier_free_bturn_info	(&(ptr->tlist[i]->info));

		snapstabremove (bturn->snap, stab);
		bturn->snap->stab = NULL;
		SPWAW_snap_free (&(bturn->snap));

		free (ptr->tlist[i]);
		for(j=i+1; j<ptr->tcnt; j++) ptr->tlist[j-1] = ptr->tlist[j];
		ptr->tlist[ptr->tcnt-1] = NULL;
		ptr->tcnt--;
		break;
	}

	// TODO: investigate usefulnes of auto-cleanup
	if (ptr->tcnt == 0) {
		free (ptr->tlist); ptr->tlist = NULL;
	} else {
		ptr->tfirst = ptr->tlist[0];
		ptr->tlast  = ptr->tlist[ptr->tcnt-1];
	}
	dossier_update_battle_info (ptr);
}

SPWAW_ERROR
dossier_del (SPWAW_DOSSIER *ptr, SPWAW_BTURN *bturn)
{
	SPWAW_BATTLE	*b = NULL;
	STRTAB		*stab = NULL;

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (bturn);
	stab = (STRTAB *)ptr->stab;

	b = dossier_find_battle (ptr, bturn->snap);
	if (b) {
		battle_del_battle_turn (b, bturn, stab);
		// TODO: investigate usefulnes of auto-cleanup
		if (b->tcnt == 0) {
			dossier_del_battle (ptr, b, stab);
		}
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_del (SPWAW_DOSSIER *ptr, SPWAW_BATTLE *battle)
{
	STRTAB	*stab = NULL;

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (battle);
	stab = (STRTAB *)ptr->stab;

	while (battle->tcnt) battle_del_battle_turn (battle, battle->tlist[0], stab);
	dossier_del_battle (ptr, battle, stab);

	return (SPWERR_OK);
}
