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
#include "dossier/dossier_file.h"
#include "snapshot/index.h"
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "common/internal.h"

SPWAW_BATTLE *
dossier_find_battle (SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap)
{
	DWORD		i;
	SPWAW_TIMESTAMP	stamp, s;

	SPWAW_date2stamp (&(snap->game.battle.data.start), &stamp);

	for (i=0; i<ptr->bcnt; i++) {
		SPWAW_date2stamp (&(ptr->blist[i]->date), &s);
		if (s == stamp) return (ptr->blist[i]);
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

	ip->pbir.fir = safe_nmalloc (SPWAW_DOSSIER_FIR, ip->pbir.fcnt); COOM (ip->pbir.fir, "player SPWAW_DOSSIER_FIR list");
	for (idx=0; idx<ip->pbir.fcnt; idx++) {
		ip->pbir.fir[idx].snap = &(bt->snap->OOBp1.battle.formations.list[idx]);
		ip->pbir.fir[idx].leader = ip->pbir.fir[idx].snap->data.leader.up->data.idx;
	}

	ip->pbir.uir = safe_nmalloc (SPWAW_DOSSIER_UIR, ip->pbir.ucnt); COOM (ip->pbir.uir, "player SPWAW_DOSSIER_UIR list");
	for (idx=0; idx<ip->pbir.ucnt; idx++) {
		ip->pbir.uir[idx].snap = &(bt->snap->OOBp1.battle.units.list[idx]);
		ip->pbir.uir[idx].fptr = &(ip->pbir.fir[ip->pbir.uir[idx].snap->data.FMID]);
	}

	ip->obir.fir = safe_nmalloc (SPWAW_DOSSIER_FIR, ip->obir.fcnt); COOM (ip->obir.fir, "opponent SPWAW_DOSSIER_FIR list");
	for (idx=0; idx<ip->obir.fcnt; idx++) {
		ip->obir.fir[idx].snap = &(bt->snap->OOBp2.battle.formations.list[idx]);
		ip->obir.fir[idx].leader = ip->obir.fir[idx].snap->data.leader.up->data.idx;
	}

	ip->obir.uir = safe_nmalloc (SPWAW_DOSSIER_UIR, ip->obir.ucnt); COOM (ip->obir.uir, "opponent SPWAW_DOSSIER_UIR list");
	for (idx=0; idx<ip->obir.ucnt; idx++) {
		ip->obir.uir[idx].snap = &(bt->snap->OOBp2.battle.units.list[idx]);
		ip->obir.uir[idx].fptr = &(ip->obir.fir[ip->obir.uir[idx].snap->data.FMID]);
	}

	return (SPWERR_OK);
}

void
dossier_free_bturn_info (SPWAW_DOSSIER_BIRS *ip)
{
	if (!ip) return;

	if (ip->pbir.uir) free (ip->pbir.uir);
	if (ip->pbir.fir) free (ip->pbir.fir);
	if (ip->obir.uir) free (ip->obir.uir);
	if (ip->obir.fir) free (ip->obir.fir);
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
		ptr->ra = safe_nmalloc (SPWAW_DOSSIER_BURA, ptr->dossier->ucnt); COOM (ptr->ra, "RA list");
		for (i=0; i<ptr->dossier->ucnt; i++) {
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
		for (i=0; i<dst->dossier->ucnt; i++) {
			dst->ra[i].src = SPWAW_BADIDX;
		}
	}
	if (dst == NULL) {
		for (i=0; i<src->dossier->ucnt; i++) {
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
		for (i=0; i<src->dossier->ucnt; i++) {
			src->ra[i].dst = dst->ra[i].src = SPWAW_BADIDX;
			src->ra[i].rpl = false;
		}
		for (f=0; f<src->dossier->fcnt; f++) {
			sfp = &(src->info_sob->pbir.fir[f]); dfp = &(dst->info_sob->pbir.fir[f]);
			log ("Src formation #%d: sfp=0x%8.8x = %s\n", f, sfp, sfp->snap->strings.name);
			log ("Dst formation #%d: dfp=0x%8.8x = %s\n", f, dfp, dfp->snap->strings.name);

			/* first assign live units */
			log ("/ Assigning live units:\n");
			for (u=0; u<sfp->snap->data.ucnt; u++) {
				i = sfp->snap->data.ulist[u]->data.idx;
				if (!src->info_eob->pbir.uir[i].snap->data.alive) continue;

				sup = &(src->info_sob->pbir.uir[i]);
				log ("| Src unit #%d: (%s, %s), ALIVE\n", i, sup->snap->strings.uid, sup->snap->data.name);

				/* find first unassigned matching unit in dst formation */
				p = NULL; k = SPWAW_BADIDX;
				for (j=0; j<dfp->snap->data.ucnt; j++) {
					k = dfp->snap->data.ulist[j]->data.idx;
					p = &(dst->info_sob->pbir.uir[k]);

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

				sup = &(src->info_sob->pbir.uir[i]);
				log ("| Src unit #%d: (%s, %s)\n", i, sup->snap->strings.uid, sup->snap->data.name);

				/* find first unassigned unit in dst formation */
				p = NULL; k = SPWAW_BADIDX;
				for (j=0; j<dfp->snap->data.ucnt; j++) {
					k = dfp->snap->data.ulist[j]->data.idx;
					p = &(dst->info_sob->pbir.uir[k]);

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
		for (i=0; i<src->dossier->ucnt; i++) {
			log ("final reassignment result: src->ra[%03.3d].dst = %03.3d, dst->ra[%03.3d].src = %03.3d, src->ra[%03.3d].rpl = %d\n",
				i, src->ra[i].dst, i, dst->ra[i].src, i, src->ra[i].rpl);
		}
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_update_dossier_info (SPWAW_DOSSIER *ptr)
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
