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
#include "dossier/dossier_file.h"
#include "snapshot/index.h"
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "common/internal.h"

SPWAW_BATTLE *
dossier_find_battle (SPWAW_DOSSIER *ptr, SPWAW_BATTLE_DATE *bdate)
{
	DWORD		i;

	for (i=0; i<ptr->bcnt; i++) {
		if (SPWAW_bdate_cmp (&(ptr->blist[i]->bdate), bdate) == 0) return (ptr->blist[i]);
	}
	return (NULL);
}

SPWAW_BATTLE *
dossier_find_battle (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap)
{
	DWORD		i;

	if (snap->game.btlidx != SPWAW_NOBTLIDX) {
		for (i=0; i<ptr->bcnt; i++) {
			if (ptr->blist[i]->bdate.btlidx == snap->game.btlidx) return (ptr->blist[i]);
		}
	} else {
		SPWAW_TIMESTAMP	stamp, s;

		SPWAW_date2stamp (&(snap->game.battle.data.bdate.date), &stamp);
		for (i=0; i<ptr->bcnt; i++) {
			SPWAW_date2stamp (&(ptr->blist[i]->bdate.date), &s);
			if (s == stamp) return (ptr->blist[i]);
		}
	}
	return (NULL);
}

SPWAW_BTURN *
dossier_find_bturn (SPWAW_BATTLE *ptr, SPWAW_SNAPSHOT *snap)
{
	DWORD		i;
	SPWAW_TIMESTAMP	stamp, s;
	int		turn, t;

	SPWAW_date2stamp (&(snap->game.battle.data.tdate.date), &stamp);
	turn = snap->game.battle.data.tdate.turn;

	for (i=0; i<ptr->tcnt; i++) {
		SPWAW_date2stamp (&(ptr->tlist[i]->tdate.date), &s);
		t = ptr->tlist[i]->tdate.turn;
		if ((s == stamp) && (t == turn)) return (ptr->tlist[i]);
	}
	return (NULL);
}

bool
battle_contains_bturn (SPWAW_BATTLE *ptr, SPWAW_BTURN *bturn)
{
	DWORD	i;

	for (i=0; i<ptr->tcnt; i++) {
		if (ptr->tlist[i] == bturn) return (true);
	}
	return (false);
}

SPWAW_ERROR
dossier_fix_battle_info (SPWAW_BTURN *bt, SPWAW_BTURN *ref)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (bt); CNULLARG (ref);

	/* Update snapshot attributes */
	rc = snapbattlefix (bt->snap, ref->snap);	ROE ("snapbattlefix()");

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_prep_bturn_info (SPWAW_BTURN *bt)
{
	SPWAW_DOSSIER_BIRS	*ip = NULL;
	USHORT			idx;

	CNULLARG (bt);
	ip = &(bt->info);

	ip->pbir_core.fcnt    = bt->snap->OOBp1.core.formations.cnt;
	ip->pbir_core.ucnt    = bt->snap->OOBp1.core.units.cnt;
	ip->pbir_support.fcnt = bt->snap->OOBp1.support.formations.cnt;
	ip->pbir_support.ucnt = bt->snap->OOBp1.support.units.cnt;
	ip->pbir_battle.fcnt  = bt->snap->OOBp1.battle.formations.cnt;
	ip->pbir_battle.ucnt  = bt->snap->OOBp1.battle.units.cnt;
	ip->obir_battle.fcnt  = bt->snap->OOBp2.battle.formations.cnt;
	ip->obir_battle.ucnt  = bt->snap->OOBp2.battle.units.cnt;

	ip->pbir_core.fir = safe_nmalloc (SPWAW_DOSSIER_FIR, ip->pbir_core.fcnt);
	COOM (ip->pbir_core.fir, "player core force SPWAW_DOSSIER_FIR list");
	for (idx=0; idx<ip->pbir_core.fcnt; idx++) {
		ip->pbir_core.fir[idx].snap = &(bt->snap->OOBp1.core.formations.list[idx]);
		ip->pbir_core.fir[idx].leader = ip->pbir_core.fir[idx].snap->data.leader.up->data.idx;
	}

	ip->pbir_core.uir = safe_nmalloc (SPWAW_DOSSIER_UIR, ip->pbir_core.ucnt);
	COOM (ip->pbir_core.uir, "player core force SPWAW_DOSSIER_UIR list");
	for (idx=0; idx<ip->pbir_core.ucnt; idx++) {
		ip->pbir_core.uir[idx].snap = &(bt->snap->OOBp1.core.units.list[idx]);
		ip->pbir_core.uir[idx].fptr = &(ip->pbir_core.fir[ip->pbir_core.uir[idx].snap->data.FMID]);
	}

	ip->pbir_support.fir = safe_nmalloc (SPWAW_DOSSIER_FIR, ip->pbir_support.fcnt);
	COOM (ip->pbir_support.fir, "player support force SPWAW_DOSSIER_FIR list");
	for (idx=0; idx<ip->pbir_support.fcnt; idx++) {
		ip->pbir_support.fir[idx].snap = &(bt->snap->OOBp1.support.formations.list[idx]);
		ip->pbir_support.fir[idx].leader = ip->pbir_support.fir[idx].snap->data.leader.up->data.idx;
	}

	ip->pbir_support.uir = safe_nmalloc (SPWAW_DOSSIER_UIR, ip->pbir_support.ucnt);
	COOM (ip->pbir_support.uir, "player support force SPWAW_DOSSIER_UIR list");
	for (idx=0; idx<ip->pbir_support.ucnt; idx++) {
		ip->pbir_support.uir[idx].snap = &(bt->snap->OOBp1.support.units.list[idx]);
		ip->pbir_support.uir[idx].fptr = &(ip->pbir_support.fir[ip->pbir_support.uir[idx].snap->data.FMID]);
	}

	ip->pbir_battle.fir = safe_nmalloc (SPWAW_DOSSIER_FIR, ip->pbir_battle.fcnt);
	COOM (ip->pbir_battle.fir, "player battle force SPWAW_DOSSIER_FIR list");
	for (idx=0; idx<ip->pbir_battle.fcnt; idx++) {
		ip->pbir_battle.fir[idx].snap = &(bt->snap->OOBp1.battle.formations.list[idx]);
		ip->pbir_battle.fir[idx].leader = ip->pbir_battle.fir[idx].snap->data.leader.up->data.idx;
	}

	ip->pbir_battle.uir = safe_nmalloc (SPWAW_DOSSIER_UIR, ip->pbir_battle.ucnt);
	COOM (ip->pbir_battle.uir, "player battle force SPWAW_DOSSIER_UIR list");
	for (idx=0; idx<ip->pbir_battle.ucnt; idx++) {
		ip->pbir_battle.uir[idx].snap = &(bt->snap->OOBp1.battle.units.list[idx]);
		ip->pbir_battle.uir[idx].fptr = &(ip->pbir_battle.fir[ip->pbir_battle.uir[idx].snap->data.FMID]);
	}

	ip->obir_battle.fir = safe_nmalloc (SPWAW_DOSSIER_FIR, ip->obir_battle.fcnt);
	COOM (ip->obir_battle.fir, "opponent battle force SPWAW_DOSSIER_FIR list");
	for (idx=0; idx<ip->obir_battle.fcnt; idx++) {
		ip->obir_battle.fir[idx].snap = &(bt->snap->OOBp2.battle.formations.list[idx]);
		ip->obir_battle.fir[idx].leader = ip->obir_battle.fir[idx].snap->data.leader.up->data.idx;
	}

	ip->obir_battle.uir = safe_nmalloc (SPWAW_DOSSIER_UIR, ip->obir_battle.ucnt);
	COOM (ip->obir_battle.uir, "opponent battle force SPWAW_DOSSIER_UIR list");
	for (idx=0; idx<ip->obir_battle.ucnt; idx++) {
		ip->obir_battle.uir[idx].snap = &(bt->snap->OOBp2.battle.units.list[idx]);
		ip->obir_battle.uir[idx].fptr = &(ip->obir_battle.fir[ip->obir_battle.uir[idx].snap->data.FMID]);
	}

	return (SPWERR_OK);
}

void
dossier_free_bturn_info (SPWAW_DOSSIER_BIRS *ip)
{
	if (!ip) return;

	if (ip->pbir_core.uir)    free (ip->pbir_core.uir);
	if (ip->pbir_core.fir)    free (ip->pbir_core.fir);
	if (ip->pbir_support.uir) free (ip->pbir_support.uir);
	if (ip->pbir_support.fir) free (ip->pbir_support.fir);
	if (ip->pbir_battle.uir)  free (ip->pbir_battle.uir);
	if (ip->pbir_battle.fir)  free (ip->pbir_battle.fir);
	if (ip->obir_battle.uir)  free (ip->obir_battle.uir);
	if (ip->obir_battle.fir)  free (ip->obir_battle.fir);
	clear_ptr (ip);
}

SPWAW_ERROR
dossier_update_battle_info (SPWAW_BATTLE *ptr)
{
	USHORT	i;

	CNULLARG (ptr); CNULLARG(ptr->dossier);

	ptr->snap = NULL;
	ptr->tfirst = ptr->tlast = NULL;
	ptr->info_sob = ptr->info_eob = NULL;

	if (!ptr->tcnt) return (SPWERR_OK);

	ptr->snap = ptr->tlist[0]->snap;
	ptr->tfirst = ptr->tlist[0];
	ptr->tlast  = ptr->tlist[ptr->tcnt-1];
	ptr->info_sob = &(ptr->tfirst->info);
	ptr->info_eob = &(ptr->tlast->info);

	if (!ptr->ra) {
		ptr->racnt = ptr->props.pc_ucnt;
		ptr->ra = safe_nmalloc (SPWAW_DOSSIER_BURA, ptr->racnt); COOM (ptr->ra, "RA list");
		for (i=0; i<ptr->racnt; i++) {
			ptr->ra[i].src = SPWAW_BADIDX;
			ptr->ra[i].dst = SPWAW_BADIDX;
			ptr->ra[i].rpl = false;
		}
	}

	return (SPWERR_OK);
}

static void
prepare_battle_rainfo (SPWAW_BATTLE *src, SPWAW_BATTLE *dst)
{
	USHORT	i;

	if (src != NULL) {
		for (i=0; i<src->racnt; i++) {
			src->ra[i].dst = SPWAW_BADIDX;
			src->ra[i].rpl = false;
		}
	}

	if (dst != NULL) {
		for (i=0; i<dst->racnt; i++) {
			dst->ra[i].src = SPWAW_BADIDX;
		}
	}
}

static bool
string_match (char *n1, char *n2)
{
	int s1 = strlen (n1);
	int s2 = strlen (n2);

	if (s1 != s2) return (false);

	while (s1) {
		s1--;
		if (n1[s1] != n2[s1]) return (false);
	}

	return (true);
}

static int
rating_worsened (SPWAW_DOSSIER_UIR *sup, SPWAW_DOSSIER_UIR *dup)
{
	int	d = 0;
	int	dp;

	/* Rally */
	dp = (dup->snap->data.ral - sup->snap->data.ral); if (dp < 0) d += 1;

	/* Infantry, Armour and Artillery */
	dp = (dup->snap->data.inf - sup->snap->data.inf); if (dp < 0) d += 1;
	dp = (dup->snap->data.arm - sup->snap->data.arm); if (dp < 0) d += 1;
	dp = (dup->snap->data.art - sup->snap->data.art); if (dp < 0) d += 1;

	return (d);
}

static int
stats_worsened (SPWAW_DOSSIER_UIR *sup, SPWAW_DOSSIER_UIR *dup)
{
	int	d = 0;
	int	dp;

	/* Experience and Morale */
	dp = (dup->snap->data.exp - sup->snap->data.exp); if (dp < 0) d += 1;
	dp = (dup->snap->data.mor - sup->snap->data.mor); if (dp < 0) d += 1;

	/* Kills */
	dp = (dup->snap->attr.gen.kills - sup->snap->attr.gen.kills); if (dp < 0) d += 1;

	return (d);
}

static double
match_score (SPWAW_BATTLE *src, USHORT si, SPWAW_BATTLE *dst, USHORT di)
{
	SPWAW_DOSSIER_UIR	*sup, *dup;
	USHORT			sfi, dfi;
	SPWAW_DOSSIER_FIR	*sfp, *dfp;
	int			score, w;

	sup = &(src->info_sob->pbir_core.uir[si]);
	sfi = sup->fptr->snap->data.idx;
	sfp = &(src->info_sob->pbir_core.fir[sfi]);

	dup = &(dst->info_sob->pbir_core.uir[di]);
	dfi = dup->fptr->snap->data.idx;
	dfp = &(dst->info_sob->pbir_core.fir[dfi]);

	RASCORETRACE2 (" \\ calculating match score for si=%05.5d, di=%05.5d\n", si, di);
	RASCORETRACE2 ("  \\ Dst (%s, %s)\n", dup->snap->strings.uid, dup->snap->data.lname);

	/* Match scoring uses the following heuristics:
	 *
	 * If the unit is alive:
	 *     10 points for lname match
	 *     10 points for FID match
	 *     10 points for UID match (? special for winSPWW2 and if FSID==0 ?)
	 *     deduct worsened ratings (ral/inf/arm/art)
	 *     deduct worsened stats distance (exp/mor/kills)
	 *
	 * If the unit is dead:
	 *     5 points for FID match
	 */

	score = 0;
	if (src->info_eob->pbir_core.uir[si].snap->data.alive) {
		if (string_match (sup->snap->data.lname, dup->snap->data.lname)) {
			score += 10;
			RASCORETRACE1 ("   \\+-- lname match! score: %d\n", score);
		}
		if (sup->fptr->snap->data.FID == dup->fptr->snap->data.FID) {
			score += 10;
			RASCORETRACE1 ("   \\+-- FID match! score: %d\n", score);
		}
		/* special rule for winSPWW2 games */
		if (src->dossier->gametype == SPWAW_GAME_TYPE_WINSPWW2) {
			if (sup->snap->data.FSID == 0) {
				if (string_match (sup->snap->strings.uid, dup->snap->strings.uid)) {
					score += 10;
					RASCORETRACE1 ("   \\+-- (winSPWW2) UID match! score: %d\n", score);
				}
			}
		}
		w = rating_worsened (sup, dup); score -= w;
		RASCORETRACE2 ("   \\+-- rating worsened: %d - score: %d\n", w, score);
		w = stats_worsened (sup, dup); score -= w;
		RASCORETRACE2 ("   \\+-- stats worsened: %d - score: %d\n", w, score);
	} else {
		if (sup->fptr->snap->data.FID == dup->fptr->snap->data.FID) {
			score += 5;
			RASCORETRACE1 ("   \\+-- FID match! score: %d\n", score);
		}
	}

	RASCORETRACE1 ("    +++ final score: %d\n", score);
	return (score);
}

SPWAW_ERROR
dossier_update_battle_rainfo (SPWAW_BATTLE *src, SPWAW_BATTLE *dst)
{
	bool			*srcf, *dstf;
	USHORT			i, j;
	double			score, s;
	USHORT			si;
	SPWAW_DOSSIER_UIR	*sup;

	if ((src == NULL) && (dst == NULL)) RWE (SPWERR_NULLARG, "unexpected NULL values for src and dst arguments");

	prepare_battle_rainfo (src, dst);

	if ((src == NULL) || (dst == NULL)) return (SPWERR_OK);

	RATRACE0 ("### dossier_update_battle_rainfo ###\n");

	safe_nalloca (srcf, bool, src->racnt); COOM (srcf, "src processing flags");
	safe_nalloca (dstf, bool, dst->racnt); COOM (dstf, "dst processing flags");

	RATRACE0 ("Assigning live units:\n");
	for (i=0; i<src->racnt; i++) {
		if (!src->info_eob->pbir_core.uir[i].snap->data.alive) continue;

		sup = &(src->info_sob->pbir_core.uir[i]);
		RATRACE3 ("| Src unit #%05.5d: (%s, %s)\n", i, sup->snap->strings.uid, sup->snap->data.lname);

		score = s = -1000000; si = SPWAW_BADIDX;
		for (j=0; j<dst->racnt; j++) {
			if (dstf[j]) continue;

			s = match_score (src, i, dst, j);
			if (s > score) {
				RATRACE3 ("| improved score for %05.5d: %.3f < %.3f\n", j, score, s);
				score = s; si = j;
			}
		}

		if (si != SPWAW_BADIDX) {
			RATRACE2 ("| Assigned %05.5d -> %05.5d\n\n", i, si);
			src->ra[i].dst = si;
			dst->ra[si].src = i;
			srcf[i] = dstf[si] = true;
		}
	}

	RATRACE0 ("Assigning remaining dead units:\n");
	for (i=0; i<src->racnt; i++) {
		if (srcf[i]) continue;

		for (j=0; j<dst->racnt; j++) {
			if (dstf[j]) continue;

			RATRACE2 ("| Assigned %05.5d -> %05.5d\n", i, j);
			src->ra[i].dst = j; src->ra[i].rpl = true;
			dst->ra[j].src = i;
			srcf[i] = dstf[j] = true;
			break;
		}
	}

	for (i=0; i<src->racnt; i++) {
		RATRACE3 ("final reassignment result: src->ra[%03.3d] .dst = %03.3d, .rpl = %d\n",
			i, src->ra[i].dst, src->ra[i].rpl);

	}
	for (i=0; i<dst->racnt; i++) {
		RATRACE2 ("final reassignment result: dst->ra[%03.3d] .src = %03.3d\n",
			i, dst->ra[i].src);
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_set_battle_props (SPWAW_BATTLE *battle)
{
	CNULLARG(battle);

	battle->props.pc_fcnt = battle->tfirst->info.pbir_core.fcnt;
	battle->props.pc_ucnt = battle->tfirst->info.pbir_core.ucnt;
	battle->props.ps_fcnt = battle->tfirst->info.pbir_support.fcnt;
	battle->props.ps_ucnt = battle->tfirst->info.pbir_support.ucnt;
	battle->props.pb_fcnt = battle->tfirst->info.pbir_battle.fcnt;
	battle->props.pb_ucnt = battle->tfirst->info.pbir_battle.ucnt;
	battle->props.ob_fcnt = battle->tfirst->info.obir_battle.fcnt;
	battle->props.ob_ucnt = battle->tfirst->info.obir_battle.ucnt;

	return (SPWERR_OK);
}


SPWAW_ERROR
dossier_update_campaign_props (SPWAW_DOSSIER *ptr)
{
	CNULLARG(ptr);

	ptr->props.OOB       = ptr->bfirst->OOB_p1;
	ptr->props.start     = ptr->bfirst->tfirst->snap->game.campaign.data.start;
	ptr->props.end       = ptr->bfirst->tfirst->snap->game.campaign.data.end;
	ptr->props.maxbtlcnt = ptr->bfirst->tfirst->snap->game.campaign.data.battles_max;
	ptr->props.ifcnt     = ptr->bfirst->props.pc_fcnt;
	ptr->props.iucnt     = ptr->bfirst->props.pc_ucnt;
	ptr->props.cfcnt     = ptr->blast->props.pc_fcnt;
	ptr->props.cucnt     = ptr->blast->props.pc_ucnt;

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_update_dossier_stats (SPWAW_DOSSIER *ptr)
{
	USHORT	i;

	CNULLARG (ptr);

	memset (&(ptr->stats), 0, sizeof(ptr->stats));
	for (i=0; i<ptr->bcnt; i++) {
		SPWAW_BATTLE *b = ptr->blist[i];
		if (b->tcnt == 0) continue;
		if (b->tlist[b->tcnt-1]->snap->game.battle.data.status == SPWAW_BTSCORE) {
			ptr->stats.concluded++;
			ptr->stats.results[b->tlist[b->tcnt-1]->snap->game.campaign.data.P1result]++;
		}
	}

	return (SPWERR_OK);
}
