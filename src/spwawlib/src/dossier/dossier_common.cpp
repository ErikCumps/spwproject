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
dossier_find_battle (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap)
{
	DWORD		i;

	if (snap->game.cbidx != SPWAW_NOCBIDX) {
		for (i=0; i<ptr->bcnt; i++) {
			if (ptr->blist[i]->cbidx == snap->game.cbidx) return (ptr->blist[i]);
		}
	} else {
		SPWAW_TIMESTAMP	stamp, s;

		SPWAW_date2stamp (&(snap->game.battle.data.start), &stamp);
		for (i=0; i<ptr->bcnt; i++) {
			SPWAW_date2stamp (&(ptr->blist[i]->date), &s);
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

	SPWAW_date2stamp (&(snap->game.battle.data.date), &stamp);

	for (i=0; i<ptr->tcnt; i++) {
		SPWAW_date2stamp (&(ptr->tlist[i]->date), &s);
		if (s == stamp) return (ptr->tlist[i]);
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
		ptr->ra = safe_nmalloc (SPWAW_DOSSIER_BURA, ptr->dossier->props.ucnt); COOM (ptr->ra, "RA list");
		for (i=0; i<ptr->dossier->props.ucnt; i++) {
			ptr->ra[i].src = SPWAW_BADIDX;
			ptr->ra[i].dst = SPWAW_BADIDX;
			ptr->ra[i].rpl = false;
		}
	}

	return (SPWERR_OK);
}

static void
dossier_update_battle_rainfo_simple (SPWAW_BATTLE *src, SPWAW_BATTLE *dst)
{
	USHORT	i;

	if (src == NULL) {
		for (i=0; i<dst->dossier->props.ucnt; i++) {
			dst->ra[i].src = SPWAW_BADIDX;
		}
	}
	if (dst == NULL) {
		for (i=0; i<src->dossier->props.ucnt; i++) {
			src->ra[i].dst = SPWAW_BADIDX;
			src->ra[i].rpl = false;
		}
	}
}

SPWAW_ERROR
dossier_update_battle_rainfo (SPWAW_BATTLE *src, SPWAW_BATTLE *dst)
{
	USHORT			f, u, i, j, k;
	SPWAW_DOSSIER_FIR	*sfp, *dfp;
	SPWAW_DOSSIER_UIR	*sup;
	SPWAW_DOSSIER_UIR	*p;

	if ((src == NULL) && (dst == NULL)) RWE (SPWERR_NULLARG, "unexpected NULL values for src and dst arguments");

	if ((src == NULL) || (dst == NULL)) {
		dossier_update_battle_rainfo_simple (src, dst);
	} else {
		log ("### dossier_update_battle_rainfo ###\n");
		for (i=0; i<src->dossier->props.ucnt; i++) {
			src->ra[i].dst = dst->ra[i].src = SPWAW_BADIDX;
			src->ra[i].rpl = false;
		}
		for (f=0; f<src->dossier->props.fcnt; f++) {
			sfp = &(src->info_sob->pbir_core.fir[f]); dfp = &(dst->info_sob->pbir_core.fir[f]);
			log ("Src formation #%d: sfp=0x%8.8x = %s\n", f, sfp, sfp->snap->strings.name);
			log ("Dst formation #%d: dfp=0x%8.8x = %s\n", f, dfp, dfp->snap->strings.name);

			/* first assign live units */
			log ("/ Assigning live units:\n");
			for (u=0; u<sfp->snap->data.ucnt; u++) {
				i = sfp->snap->data.ulist[u]->data.idx;
				if (!src->info_eob->pbir_core.uir[i].snap->data.alive) continue;

				sup = &(src->info_sob->pbir_core.uir[i]);
				log ("| Src unit #%d: (%s, %s), ALIVE\n", i, sup->snap->strings.uid, sup->snap->data.name);

				/* find first unassigned matching unit in dst formation */
				p = NULL; k = SPWAW_BADIDX;
				for (j=0; j<dfp->snap->data.ucnt; j++) {
					k = dfp->snap->data.ulist[j]->data.idx;
					p = &(dst->info_sob->pbir_core.uir[k]);

					log ("| Checking dst unit #%d: (%s, %s), dst->ra[k].src = %d\n", k, p->snap->strings.uid, p->snap->data.name, dst->ra[k].src);
					if (strcmp (sup->snap->data.name, p->snap->data.name) == 0) {
						if (dst->ra[k].src == SPWAW_BADIDX) {
							log ("| -> found 1st match!\n");
							break;
						} else {
							log ("| -> found match but target already assigned.\n");
						}
					} else {
						p = NULL;
					}
				}
				if (p) {
					if (i == k)
						log ("RESULT: assigned %d -> %d\n", i, k);
					else
						log ("RESULT: reassigned %d -> %d\n", i, k);
					src->ra[i].dst = k;
					dst->ra[k].src = i;
				} else {
					log ("RESULT: no match found, unit considered dead!\n");
				}
			}

			/* next process remaining (dead or assumed dead) units */
			log ("/ Assigning remaining units:\n");
			for (u=0; u<sfp->snap->data.ucnt; u++) {
				i = sfp->snap->data.ulist[u]->data.idx;
				if (src->ra[i].dst != SPWAW_BADIDX) continue;

				sup = &(src->info_sob->pbir_core.uir[i]);
				log ("| Src unit #%d: (%s, %s)\n", i, sup->snap->strings.uid, sup->snap->data.name);

				/* find first unassigned unit in dst formation */
				p = NULL; k = SPWAW_BADIDX;
				for (j=0; j<dfp->snap->data.ucnt; j++) {
					k = dfp->snap->data.ulist[j]->data.idx;
					p = &(dst->info_sob->pbir_core.uir[k]);

					log ("| Checking dst unit #%d: (%s, %s) dst->ra[k].src = %d\n", k, p->snap->strings.uid, p->snap->data.name, dst->ra[k].src);
					if (dst->ra[k].src == SPWAW_BADIDX) {
						log ("| -> found 1st unassigned unit!\n");
						break;
					}
				}
				if (p) {
					if (i == k)
						log ("RESULT: assigned %d -> %d\n", i, k);
					else
						log ("RESULT: reassigned %d -> %d\n", i, k);
					src->ra[i].dst = k;
					dst->ra[k].src = i;
					src->ra[i].rpl = true;
				} else {
					log ("INTERNAL ERROR: more assigned dst units than src units!!!\n");
					abort();
				}
			}
		}
		for (i=0; i<src->dossier->props.ucnt; i++) {
			log ("final reassignment result: src->ra[%03.3d].dst = %03.3d, dst->ra[%03.3d].src = %03.3d, src->ra[%03.3d].rpl = %d\n",
				i, src->ra[i].dst, i, dst->ra[i].src, i, src->ra[i].rpl);
		}
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_set_campaign_props (SPWAW_DOSSIER *ptr)
{
	SPWAW_BATTLE	*b;

	CNULLARG(ptr);

	if (!ptr->bcnt) RWE (SPWERR_FAILED, "no battle loaded");

	b = ptr->blist[0];

	ptr->props.OOB     = b->OOB_p1;
	ptr->props.fcnt    = b->tlist[0]->info.pbir_core.fcnt;
	ptr->props.ucnt    = b->tlist[0]->info.pbir_core.ucnt;
	ptr->props.start   = b->tlist[0]->snap->game.campaign.data.start;
	ptr->props.end     = b->tlist[0]->snap->game.campaign.data.end;
	ptr->props.maxbcnt = b->tlist[0]->snap->game.campaign.data.battles_max;

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
