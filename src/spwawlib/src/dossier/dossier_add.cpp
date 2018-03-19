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
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "common/internal.h"

#define	LISTINC	8

static SPWAW_ERROR
dossier_list_expand (SPWAW_DOSSIER *ptr)
{
	SPWAW_BATTLE	**list = NULL;

	CNULLARG (ptr);

	if (ptr->bcnt >= ptr->blen) {
		ptr->blen += LISTINC;
		list = safe_nmalloc (SPWAW_BATTLE *, ptr->blen); COOM (list, "SPWAW_BATTLE* list");

		if (ptr->blist) {
			memcpy (list, ptr->blist, ptr->bcnt * sizeof (SPWAW_BATTLE *));
			free (ptr->blist);
		}
		ptr->blist = list;
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_new_battle (SPWAW_BATTLE **ptr, SPWAW_SNAPSHOT *snap, STRTAB *stab)
{
	SPWAW_BATTLE	*p = NULL;

	CNULLARG (ptr); CNULLARG (snap);
	*ptr = NULL;

	p = safe_malloc (SPWAW_BATTLE); COOM (p, "SPWAW_BATTLE");

	p->prev = p->next = NULL;

	p->snap = snap;

	p->date		= p->snap->game.battle.data.start;
	p->location	= STRTAB_add (stab, p->snap->game.battle.data.location);
	p->OOB_p1	= p->snap->game.battle.data.OOB_p1;
	p->OOB_p2	= p->snap->game.battle.data.OOB_p2;
	p->miss_p1	= STRTAB_add (stab, p->snap->game.battle.strings.miss_p1);
	p->miss_p2	= STRTAB_add (stab, p->snap->game.battle.strings.miss_p2);
	p->meeting	= p->snap->game.battle.data.meeting;

	*ptr = p;

	return (SPWERR_OK);
}

static int
sort_dossier (const void *a, const void *b)
{
	SPWAW_BATTLE	*fa = *((SPWAW_BATTLE **)a);
	SPWAW_BATTLE	*fb = *((SPWAW_BATTLE **)b);
	SPWAW_TIMESTAMP	sa, sb;

	SPWAW_date2stamp (&(fa->date), &sa);
	SPWAW_date2stamp (&(fb->date), &sb);
	return ((sa==sb)?0:((sa<sb)?-1:+1));
}

static SPWAW_BATTLE *
link_battle (SPWAW_BATTLE *p, SPWAW_BATTLE *pp)
{
	p->prev = pp;
	p->next = NULL;

	if (p->prev) p->prev->next = p;

	return (p);
}

static SPWAW_ERROR
dossier_add_battle (SPWAW_DOSSIER *ptr, SPWAW_BATTLE *b)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	USHORT		idx;
	SPWAW_BATTLE	*pp;

	CNULLARG (ptr); CNULLARG (b);

	rc = dossier_list_expand (ptr);
	ROE ("dossier_list_expand()");

	b->dossier = ptr;
	ptr->blist[ptr->bcnt++] = b;

	qsort (ptr->blist, ptr->bcnt, sizeof (SPWAW_BATTLE *), sort_dossier);

	pp=NULL;
	for (idx=0; idx<ptr->bcnt; idx++) {
		pp = link_battle (ptr->blist[idx], pp);
	}
	ptr->bfirst = ptr->blist[0];
	ptr->blast  = ptr->blist[ptr->bcnt-1];

	return (SPWERR_OK);
}

static SPWAW_ERROR
battle_list_expand (SPWAW_BATTLE *ptr)
{
	SPWAW_BTURN	**list = NULL;

	CNULLARG (ptr);

	if (ptr->tcnt >= ptr->tlen) {
		ptr->tlen += LISTINC;
		list = safe_nmalloc (SPWAW_BTURN *, ptr->tlen); COOM (list, "SPWAW_BTURN* list");

		if (ptr->tlist) {
			memcpy (list, ptr->tlist, ptr->tcnt * sizeof (SPWAW_BTURN *));
			free (ptr->tlist);
		}
		ptr->tlist = list;
	}

	return (SPWERR_OK);
}

static int
sort_battle (const void *a, const void *b)
{
	SPWAW_BTURN	*fa = *((SPWAW_BTURN **)a);
	SPWAW_BTURN	*fb = *((SPWAW_BTURN **)b);
	SPWAW_TIMESTAMP	sa, sb;

	SPWAW_date2stamp (&(fa->date), &sa);
	SPWAW_date2stamp (&(fb->date), &sb);
	return ((sa==sb)?0:((sa<sb)?-1:+1));
}

static SPWAW_BTURN *
link_bturn (SPWAW_BTURN *p, SPWAW_BTURN *pp)
{
	p->prev = pp;
	p->next = NULL;

	if (p->prev) p->prev->next = p;

	return (p);
}

static SPWAW_ERROR
battle_add_bturn (SPWAW_BATTLE *ptr, SPWAW_SNAPSHOT *snap, STRTAB *stab, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BTURN	*bt = NULL;
	USHORT		idx;
	SPWAW_BTURN	*pp;

	CNULLARG (ptr); CNULLARG (snap); CNULLARG (bturn);
	*bturn = NULL;

	bt = safe_malloc (SPWAW_BTURN); COOM (bt, "SPWAW_BTURN");

	bt->prev = bt->next = NULL;

	bt->battle = ptr;

	bt->snap = snap;
	rc = snapstabmerge (bt->snap, stab);
	ROE ("snapstabmerge()");

	bt->date  = bt->snap->game.battle.data.date;
	bt->turn  = bt->snap->game.battle.data.turn;
	bt->info.pbir.fcnt = bt->snap->OOBp1.battle.formations.cnt;
	bt->info.pbir.ucnt = bt->snap->OOBp1.battle.units.cnt;
	bt->info.obir.fcnt = bt->snap->OOBp2.battle.formations.cnt;
	bt->info.obir.ucnt = bt->snap->OOBp2.battle.units.cnt;

	rc = dossier_prep_bturn_info (bt);
	ROE ("dossier_prep_bturn_info()");

	rc = battle_list_expand (ptr);
	ROE ("battle_list_expand()");

	bt->battle = ptr;
	ptr->tlist[ptr->tcnt++] = bt;

	qsort (ptr->tlist, ptr->tcnt, sizeof (SPWAW_BTURN *), sort_battle);

	pp=NULL;
	for (idx=0; idx<ptr->tcnt; idx++) {
		pp = link_bturn (ptr->tlist[idx], pp);
	}
	ptr->tfirst = ptr->tlist[0];
	ptr->tlast  = ptr->tlist[ptr->tcnt-1];

	rc = dossier_fix_battle_info (bt, ptr->tfirst);
	ROE ("dossier_fix_battle_info()");

	if (ptr->tfirst == bt) {
		for (idx=1; idx<ptr->tcnt; idx++) {
			rc = dossier_fix_battle_info (ptr->tlist[idx], ptr->tfirst);
			ROE ("dossier_fix_battle_info()");
		}
	}

	*bturn = bt;

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_add (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BATTLE	*b = NULL;
	SPWAW_BTURN	*t = NULL;
	STRTAB		*stab = NULL;
	bool		empty;

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (snap); CNULLARG (bturn);
	*bturn = NULL;

	stab = (STRTAB *)ptr->stab;

	empty = (ptr->bcnt == 0);

	b = dossier_find_battle (ptr, snap);
	if (!b) {
		rc = dossier_new_battle (&b, snap, stab);
		ROE ("dossier_new_battle()");

		rc = dossier_add_battle (ptr, b);
		ROE ("dossier_add_battle()");
	} else {
		// TODO: detected duplicate insertion: add overwrite flag?
		t = dossier_find_bturn (b, snap);
		if (t) RWE (SPWERR_DUPTURN, "detected duplicate turn insertion");
	}

	rc = battle_add_bturn (b, snap, stab, &t);
	ROE ("battle_add_bturn()");

	if (empty && (ptr->type == SPWAW_CAMPAIGN_DOSSIER))	{
		ptr->OOB = snap->OOBp1.OOB;
		ptr->fcnt = snap->OOBp1.core.formations.cnt;
		ptr->ucnt = snap->OOBp1.core.units.cnt;
	}

	rc = dossier_update_battle_info (b);
	ROE ("dossier_update_battle_info()");

	rc = dossier_update_battle_rainfo (b->prev, b);
	ROE ("dossier_update_battle_rainfo()");

	rc = dossier_update_battle_rainfo (b, b->next);
	ROE ("dossier_update_battle_rainfo()");

	*bturn = t;

	return (SPWERR_OK);
}

