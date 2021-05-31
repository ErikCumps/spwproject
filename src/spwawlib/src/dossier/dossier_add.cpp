/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "spwoob/spwoob_list.h"
#include "snapshot/snapshot.h"
#include "gamefile/savegame_descriptor.h"
#include "strtab/strtab.h"
#include "uht/uht.h"
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
dossier_new_battle (SPWAW_BATTLE **ptr, SPWAW_SNAPSHOT *snap, const char *name, STRTAB *stab, SPWAW_DOSSIER_TYPE type)
{
	SPWAW_BATTLE	*p = NULL;

	CNULLARG (ptr); CNULLARG (snap);
	*ptr = NULL;

	p = safe_malloc (SPWAW_BATTLE); COOM (p, "SPWAW_BATTLE");

	p->prev = p->next = NULL;

	p->snap = snap;

	if (name) {
		p->name	= STRTAB_add (stab, (char *)name);
	} else {
		p->name	= NULL;
	}

	if (type != SPWAW_STDALONE_DOSSIER) {
		p->bdate.btlidx = p->snap->game.btlidx;
	} else {
		p->bdate.btlidx = SPWAW_NOBTLIDX;
	}
	p->bdate.date	= p->snap->game.battle.data.bdate.date;

	p->location_data= STRTAB_add (stab, p->snap->game.battle.data.location);
	p->location	= STRTAB_add (stab, p->location_data);
	p->oobdat	= p->snap->oobdat;
	p->OOB_p1	= p->snap->game.battle.data.OOB_p1;
	p->OOB_p2	= p->snap->game.battle.data.OOB_p2;
	p->miss_p1	= STRTAB_add (stab, p->snap->game.battle.strings.miss_p1);
	p->miss_p2	= STRTAB_add (stab, p->snap->game.battle.strings.miss_p2);
	p->meeting	= p->snap->game.battle.data.meeting;

	*ptr = p;

	return (SPWERR_OK);
}

static int
sort_dossier_campaign (const void *a, const void *b)
{
	SPWAW_BATTLE	*fa = *((SPWAW_BATTLE **)a);
	SPWAW_BATTLE	*fb = *((SPWAW_BATTLE **)b);

	return (SPWAW_bdate_cmp (&(fa->bdate), &(fb->bdate)));
}

static int
sort_dossier_stdalone (const void *a, const void *b)
{
	SPWAW_BATTLE	*fa = *((SPWAW_BATTLE **)a);
	SPWAW_BATTLE	*fb = *((SPWAW_BATTLE **)b);

	SPWAW_TIMESTAMP	sa, sb;
	SPWAW_date2stamp (&(fa->bdate.date), &sa); sa = SPWAW_PURE_TIMESTAMP(sa);
	SPWAW_date2stamp (&(fb->bdate.date), &sb); sb = SPWAW_PURE_TIMESTAMP(sb);

	return ((fa->name==fb->name)?(sa==sb)?0:((sa<sb)?-1:+1):((fa->name==NULL)?-1:((fb->name==NULL)?+1:strcmp(fa->name, fb->name))));
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
	unsigned long	oobidx;

	CNULLARG (ptr); CNULLARG (b);

	rc = dossier_list_expand (ptr);
	ROE ("dossier_list_expand()");

	rc = SPWOOB_LIST_add (ptr->oobdata, b->oobdat, &oobidx);
	ROE ("SPWOOB_LIST_add()");

	rc = SPWOOB_LIST_idx2spwoob (ptr->oobdata, oobidx, &(b->oobdat));
	ROE ("SPWOOB_LIST_idx2spwoob()");

	b->dossier = ptr;
	ptr->blist[ptr->bcnt++] = b;

	qsort (ptr->blist, ptr->bcnt, sizeof (SPWAW_BATTLE *),
		(ptr->type != SPWAW_STDALONE_DOSSIER) ? sort_dossier_campaign : sort_dossier_stdalone);

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
	int	ta = (*((SPWAW_BTURN **)a))->tdate.turn;
	int	tb = (*((SPWAW_BTURN **)b))->tdate.turn;

	return ((ta==tb)?0:((ta<tb)?-1:+1));
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

	rc = SPWOOB_compare (snap->oobdat, ptr->oobdat);
	ROE ("SPWOOB_compare()");

	bt = safe_malloc (SPWAW_BTURN); COOM (bt, "SPWAW_BTURN");

	bt->prev = bt->next = NULL;

	bt->battle = ptr;

	bt->snap = snap;
	rc = snapstabmerge (bt->snap, stab);
	ROE ("snapstabmerge()");

	bt->tdate = bt->snap->game.battle.data.tdate;

	rc = dossier_prep_bturn_info (bt);
	ROE ("dossier_prep_bturn_info()");

	rc = battle_list_expand (ptr);
	ROE ("battle_list_expand()");

	bt->battle = ptr;
	ptr->tlist[ptr->tcnt++] = bt;

	if (ptr->tcnt == 1) {
		rc = snap_mapref_create (bt->snap, &(ptr->map.raw), &(ptr->map.map));
		ROE ("snap_mapref_create()");
	} else {
		rc = snap_mapref_set (&(ptr->map.raw), &(ptr->map.map), bt->snap);
		ROE ("snap_mapref_set()");
	}

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

static SPWAW_ERROR
dossier_make_battle (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, const char *name, SPWAW_BATTLE **battle)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BATTLE	*b = NULL;
	STRTAB		*stab = NULL;

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (snap); CNULLARG (battle);
	*battle = NULL;

	stab = (STRTAB *)ptr->stab;

	rc = dossier_new_battle (&b, snap, name, stab, ptr->type);
	ROE ("dossier_new_battle()");

	rc = dossier_add_battle (ptr, b);
	ROE ("dossier_add_battle()");

	*battle = b;

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_add_campaign_battle_turn (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	bool		newb;
	SPWAW_BATTLE	*b = NULL;
	SPWAW_BTURN	*t = NULL;

	CNULLARG (stab);

	newb = false;
	b = dossier_find_battle (ptr, snap);
	if (!b) {
		newb = true;
		rc = dossier_make_battle (ptr, snap, NULL, &b);
		ROE ("dossier_make_battle()");
	} else {
		// TODO: detected duplicate insertion: add overwrite flag?
		t = dossier_find_bturn (b, snap);
		if (t) RWE (SPWERR_DUPTURN, "detected duplicate turn insertion");
	}

	rc = battle_add_bturn (b, snap, stab, &t);
	ROE ("battle_add_bturn()");

	rc = dossier_set_battle_props (b);
	ROE ("dossier_set_battle_props()");

	rc = dossier_update_campaign_props (ptr);
	ROE ("dossier_update_campaign_props()");

	rc = dossier_update_battle_info (b);
	ROE ("dossier_update_battle_info()");

	if (newb) {
		rc = dossier_update_battle_rainfo (b->prev, b);
		ROE ("dossier_update_battle_rainfo()");

		rc = dossier_update_battle_rainfo (b, b->next);
		ROE ("dossier_update_battle_rainfo()");
	}

	/* UHT now also considers in-battle status, which means that adding a turn to an existing battle
	 * can also result in a different final in-battle status, so a rebuild is always required.
	 * Maybe this can be optimized later?
	 */
	rc = UHT_rebuild (&(ptr->uht)); ROE ("UHT_rebuild()");

	if (bturn) *bturn = t;
	return (rc);
}


SPWAW_ERROR
dossier_add_to_campaign (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	STRTAB			*stab = NULL;
	bool			finalized;
	SPWAW_SNAPSHOT_INFO	info;
	char			base[MAX_PATH+1];
	char			filename[MAX_PATH+1];

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (snap); CNULLARG (bturn);
	*bturn = NULL;

	// Validate dossier type
	if (ptr->type == SPWAW_STDALONE_DOSSIER) {
		RWE (SPWERR_BADDTYPE, "this dossier does not allow adding campaign snapshots");
	}

	stab = (STRTAB *)ptr->stab;

	// If this is a turn #0 megacam savegame, try to finalized the megacam battle
	finalized = false;
	if ((ptr->type == SPWAW_MEGACAM_DOSSIER) && (snap->raw.game.battle.turn == 0)) {
		rc = dossier_finalize_megacam_battle (ptr, snap, bturn);
		if (!HASERROR) {
			finalized = true;
		} else if (rc != SPWERR_MEGACAM_NOT_FINALIZED) {
			ROE ("dossier_finalize_megacam_battle()");
		}
	}

	if (!finalized) {
		rc = dossier_add_campaign_battle_turn (ptr, snap, bturn, stab);
		ROE ("dossier_add_campaign_battle_turn()");
	}

	rc = dossier_update_dossier_stats (ptr);
	ROE ("dossier_update_dossier_stats()");

	if (ptr->type == SPWAW_MEGACAM_DOSSIER) {
		/* Cleanup, if needed */
		savegame_descriptor_clear (ptr->tracking.sgd, stab);
		if (ptr->tracking.filename) {
			STRTAB_del (stab, ptr->tracking.filename); ptr->tracking.filename = NULL;
		}

		/* Obtain snaphot information */
		rc = SPWAW_snap_info (&snap, &info);
		ROE ("SPWAW_snap_info()");

		if (!gamefile_basename (info.gametype, info.savetype, info.file, base, sizeof(base))) {
			RWE (SPWERR_FAILED, "failed to determine savegame basename");
		}

		/* Update savegame tracking information */
		rc = savegame_descriptor_init (ptr->tracking.sgd, info.gametype, info.savetype, ptr->oobdir, info.path, base, stab);
		ROE ("SPWAW_savegame_descriptor_init()");

		clear_ptr (filename); snprintf (filename, sizeof (filename) - 1, "%s\\%s", info.path, info.file);
		ptr->tracking.filename = STRTAB_add (stab, filename);
		ptr->tracking.filedate = info.filedate;
	}

	// snapshots that are used to finalize battles must be cleaned up
	if (finalized) {
		SPWAW_snap_free (&snap);
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_add_new_battle (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, const char *name, SPWAW_BATTLE **battle)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BATTLE	*b = NULL;

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (snap); CNULLARG (battle);
	*battle = NULL;

	// Validate dossier type
	if (ptr->type != SPWAW_STDALONE_DOSSIER) {
		RWE (SPWERR_BADDTYPE, "this dossier does not allow adding standalone battles");
	}

	rc = dossier_make_battle (ptr, snap, name, &b);
	ROE ("dossier_make_battle()");

	*battle = b;

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_add_to_battle (SPWAW_BATTLE *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWAW_BTURN	*t = NULL;
	STRTAB		*stab = NULL;

	CSPWINIT;
	CNULLARG (ptr); CNULLARG (ptr->dossier); CNULLARG (snap); CNULLARG (bturn);
	*bturn = NULL;

	// Validate dossier type
	if (ptr->dossier->type != SPWAW_STDALONE_DOSSIER) {
		RWE (SPWERR_BADDTYPE, "this dossier does not allow adding standalone battle snapshots");
	}

	stab = (STRTAB *)ptr->dossier->stab;

	// TODO: detected duplicate insertion: add overwrite flag?
	t = dossier_find_bturn (ptr, snap);
	if (t) RWE (SPWERR_DUPTURN, "detected duplicate turn insertion");

	rc = battle_add_bturn (ptr, snap, stab, &t);
	ROE ("battle_add_bturn()");

	rc = dossier_update_battle_info (ptr);
	ROE ("dossier_update_battle_info()");

	rc = dossier_update_dossier_stats (ptr->dossier);
	ROE ("dossier_update_dossier_stats()");

	*bturn = t;

	return (SPWERR_OK);
}
