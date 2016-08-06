/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/snapshot.h"
#include "common/internal.h"
#include "translate.h"

static SPWAW_ERROR
snapbfix_game (SPWAW_SNAPSHOT *ptr, SPWAW_SNAPSHOT *ref)
{
	CNULLARG (ptr); CNULLARG (ref);

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapbfix_oob_data (SPWAW_SNAP_OOB_FORCE *ptr, SPWAW_SNAP_OOB_FORCE *ref)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*up;
	SPWAW_SNAP_OOB_UEL	*rp;

	CNULLARG (ptr); CNULLARG (ref);

	/* Update battle ranks */
	for (i=0; i<ptr->units.cnt; i++) {
		up = &(ptr->units.list[i]);
		rp = &(ref->units.list[i]);

		if ((ptr == ref) || (i >= ref->units.cnt)) {
			up->data.brank = up->data.rank;
		} else {
			up->data.brank = rp->data.rank;
		}
		// Also update rank string:
		up->strings.rank = rank2str (up->data.brank);
	}

	return (rc);
}

static SPWAW_ERROR
snapbfix_oob_attrs (SPWAW_SNAP_OOB_FORCE *ptr, SPWAW_SNAP_OOB_FORCE *ref)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*up;

	CNULLARG (ptr); CNULLARG (ref);

	/* Fix attributes for entire force */
	memset (ptr->attr.mmas_uexp.rnk, 0, sizeof (ptr->attr.mmas_uexp.rnk));
	memset (ptr->attr.mmas_umor.rnk, 0, sizeof (ptr->attr.mmas_umor.rnk));
	memset (ptr->attr.mmas_usup.rnk, 0, sizeof (ptr->attr.mmas_usup.rnk));
	memset (ptr->attr.mmas_ural.rnk, 0, sizeof (ptr->attr.mmas_ural.rnk));
	memset (ptr->attr.mmas_uinf.rnk, 0, sizeof (ptr->attr.mmas_uinf.rnk));
	memset (ptr->attr.mmas_uarm.rnk, 0, sizeof (ptr->attr.mmas_uarm.rnk));
	memset (ptr->attr.mmas_uart.rnk, 0, sizeof (ptr->attr.mmas_uart.rnk));
	memset (ptr->attr.mmas_uready.rnk, 0, sizeof (ptr->attr.mmas_uready.rnk));
	memset (ptr->attr.mmas_ukills.rnk, 0, sizeof (ptr->attr.mmas_ukills.rnk));
	memset (ptr->attr.mmas_ucost.rnk, 0, sizeof (ptr->attr.mmas_ucost.rnk));
	memset (ptr->attr.mmas_uspeed.rnk, 0, sizeof (ptr->attr.mmas_uspeed.rnk));

	for (i=0; i<ptr->units.cnt; i++) {
		up = &(ptr->units.list[i]);

		snapint_addmmas (&(ptr->attr.mmas_uexp.rnk[up->data.brank]), up->data.exp, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_umor.rnk[up->data.brank]), up->data.mor, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_usup.rnk[up->data.brank]), up->data.sup, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_ural.rnk[up->data.brank]), up->data.ral, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_uinf.rnk[up->data.brank]), up->data.inf, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_uarm.rnk[up->data.brank]), up->data.arm, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_uart.rnk[up->data.brank]), up->data.art, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_uready.rnk[up->data.brank]), up->attr.gen.ready * 100.0, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_ukills.rnk[up->data.brank]), up->attr.gen.kills, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_ucost.rnk[up->data.brank]), up->data.cost, up->data.idx);
		snapint_addmmas (&(ptr->attr.mmas_uspeed.rnk[up->data.brank]), up->data.speed, up->data.idx);
	}

	return (rc);
}

static SPWAW_ERROR
snapbfix_oob_stats (SPWAW_SNAP_OOB_FORCE *ptr, SPWAW_SNAP_OOB_FORCE *ref)
{
	SPWAW_ERROR		rc = SPWERR_OK;
	DWORD			i;
	SPWAW_SNAP_OOB_UEL	*up;

	CNULLARG (ptr); CNULLARG (ref);

	/* Fix statistics for entire force */
	memset (ptr->stats.ustats.rnk_cnt, 0, sizeof (ptr->stats.ustats.rnk_cnt));
	memset (ptr->stats.ustats.rnk_loss, 0, sizeof (ptr->stats.ustats.rnk_loss));

	for (i=0; i<ptr->units.cnt; i++) {
		up = &(ptr->units.list[i]);

		ptr->stats.ustats.rnk_cnt[up->data.brank]++;
		ptr->stats.ustats.rnk_loss[up->data.brank] += up->attr.gen.losses;
	}

	return (rc);
}

static SPWAW_ERROR
snapbfix_oobp1 (SPWAW_SNAPSHOT *ptr, SPWAW_SNAPSHOT *ref)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr); CNULLARG (ref);

	rc = snapbfix_oob_data (&(ptr->OOBp1.battle), &(ref->OOBp1.battle));	ROE ("snapbfix_oob_attrs(OOBp1.battle)");
	rc = snapbfix_oob_attrs (&(ptr->OOBp1.battle), &(ref->OOBp1.battle));	ROE ("snapbfix_oob_attrs(OOBp1.battle)");
	rc = snapbfix_oob_stats (&(ptr->OOBp1.battle), &(ref->OOBp1.battle));	ROE ("snapbfix_oob_stats(OOBp1.battle)");

	rc = snapbfix_oob_data (&(ptr->OOBp1.core), &(ref->OOBp1.core));	ROE ("snapbfix_oob_attrs(OOBp1.core)");
	rc = snapbfix_oob_attrs (&(ptr->OOBp1.core), &(ref->OOBp1.core));	ROE ("snapbfix_oob_attrs(OOBp1.core)");
	rc = snapbfix_oob_stats (&(ptr->OOBp1.core), &(ref->OOBp1.core));	ROE ("snapbfix_oob_stats(OOBp1.core)");

	rc = snapbfix_oob_data (&(ptr->OOBp1.support), &(ref->OOBp1.support));	ROE ("snapbfix_oob_attrs(OOBp1.support)");
	rc = snapbfix_oob_attrs (&(ptr->OOBp1.support), &(ref->OOBp1.support));	ROE ("snapbfix_oob_attrs(OOBp1.support)");
	rc = snapbfix_oob_stats (&(ptr->OOBp1.support), &(ref->OOBp1.support));	ROE ("snapbfix_oob_stats(OOBp1.support)");

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapbfix_oobp2 (SPWAW_SNAPSHOT *ptr, SPWAW_SNAPSHOT *ref)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr); CNULLARG (ref);

	rc = snapbfix_oob_data (&(ptr->OOBp2.battle), &(ref->OOBp2.battle));	ROE ("snapbfix_oob_data(OOBp2.battle)");
	rc = snapbfix_oob_attrs (&(ptr->OOBp2.battle), &(ref->OOBp2.battle));	ROE ("snapbfix_oob_attrs(OOBp2.battle)");
	rc = snapbfix_oob_stats (&(ptr->OOBp2.battle), &(ref->OOBp2.battle));	ROE ("snapbfix_oob_stats(OOBp2.battle)");

	return (SPWERR_OK);
}

SPWAW_ERROR
snapbattlefix (SPWAW_SNAPSHOT *ptr, SPWAW_SNAPSHOT *ref)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	CNULLARG (ptr); CNULLARG (ref);

	rc = snapbfix_game (ptr, ref);	ROE ("snapbfix_game()");
	rc = snapbfix_oobp1 (ptr, ref);	ROE ("snapbfix_oobp1()");
	rc = snapbfix_oobp2 (ptr, ref);	ROE ("snapbfix_oobp2()");

	return (SPWERR_OK);

}
