/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V10 snapshot.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapfile_v10.h"
#include "snapshot/index.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "common/types.h"

SPWAW_ERROR
snapshot_load_v10_info_header (int fd, SNAP_INFO *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_INFO_V10	hdr_v10;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v10, sizeof (SNAP_INFO_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot info hdr v10) failed", handle_error);

	/* The V10 snapshot info header only lacks the battle and game types at the end.
	 * The only supported battle type for V10 snapshots is the SPWAW_CAMPAIGN_BATTLE,
	 * the only supported game type for V10 snapshots is the SPWAW_GAME_TYPE_SPWAW,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, &hdr_v10, sizeof (SNAP_INFO_V10));
	hdr->type = SPWAW_CAMPAIGN_BATTLE;
	hdr->gametype = SPWAW_GAME_TYPE_SPWAW;

handle_error:
	return (rc);
}

SPWAW_ERROR
snapshot_load_v10_oob_header (int fd, SNAP_OOBHDR *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_OOBHDR_V10	hdr_v10;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v10, sizeof (SNAP_OOBHDR_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot OOB data hdr v10) failed", handle_error);

	/* The V10 snapshot OOB data header has an unused fstart field. */
	hdr->fcnt  = hdr_v10.fcnt;
	hdr->fpos  = hdr_v10.fpos;
	hdr->fsize = hdr_v10.fsize;
	hdr->fcomp = hdr_v10.fcomp;
	hdr->ucnt  = hdr_v10.ucnt;
	hdr->upos  = hdr_v10.upos;
	hdr->usize = hdr_v10.usize;
	hdr->ucomp = hdr_v10.ucomp;
	hdr->lcnt  = hdr_v10.lcnt;
	hdr->lpos  = hdr_v10.lpos;
	hdr->lsize = hdr_v10.lsize;
	hdr->lcomp = hdr_v10.lcomp;
	hdr->pcnt  = hdr_v10.pcnt;
	hdr->ppos  = hdr_v10.ppos;
	hdr->psize = hdr_v10.psize;
	hdr->pcomp = hdr_v10.pcomp;

handle_error:
	return (rc);
}

#define	copyOU(name)	uel->##name = uel_v10.##name

SPWAW_ERROR
snapshot_load_v10_oob_uel (SBR *sbr, SNAP_OOB_UEL *uel)
{
	SNAP_OOB_UEL_V10	uel_v10;

	CNULLARG (sbr); CNULLARG (uel);

	if (sbread (sbr, (char *)&uel_v10, sizeof (SNAP_OOB_UEL_V10)) != sizeof (SNAP_OOB_UEL_V10))
			RWE (SPWERR_FRFAILED, "sbread(v10 unit data)");

	/* The V10 snapshot unit element lacks the detected unit type and abandonment status. */
	uel->dutype = UT_UNKNOWN;
	uel->aband = AS_NONE;

	copyOU (RID); copyOU (FRID); copyOU (FMID); copyOU (FSID);
	copyOU (name);
	copyOU (classID); copyOU (OOB); copyOU (OOBrid);
	copyOU (size); copyOU (cost); copyOU (survive); copyOU (leader);
	copyOU (exp); copyOU (mor); copyOU (sup); copyOU (status); copyOU (entr);
	copyOU (smkdev); copyOU (smkammo); copyOU (crew);
	copyOU (range); copyOU (stance_x); copyOU (stance_y);
	copyOU (loader); copyOU (load_cap); copyOU (load_cost);
	copyOU (contact); copyOU (rof); copyOU (tgt); copyOU (rf); copyOU (fc); copyOU (iv);
	copyOU (swim); copyOU (men); copyOU (men_ori); copyOU (speed); copyOU (moves);
	copyOU (damage); copyOU (movdir); copyOU (shtdir); copyOU (target); copyOU (UTGidx);
//	copyOU (SPECIAL_OU); copyOU (SPECIAL[0]); copyOU (SPECIAL[1]);
//	copyOU (SPECIAL[2]); copyOU (SPECIAL[3]); copyOU (SPECIAL[4]);

	return (SPWERR_OK);
}

/* Unit leader and crew linkage determination:
 *
 * <unit has crew?>
 * 	NO : <unit has leader?>
 * 		YES: leader found
 * 		NO : use unit #id as leader #id
 * 	YES: <crew has unit?>
 * 		NO : INVALID UNIT!
 * 		YES: <crew unit == unit?>
 * 			YES: <crew has no leader?>
 * 				NO : leader found
 * 				YES: [GOTO <unit has no leader?>]
 * 			NO : [GOTO <unit has no leader?>]
 */
static inline bool
snapshot_legacy_determine_ldrcrw (SPWAW_SNAP_OOB_URAW *ptr, int i, USHORT &ldr, USHORT &crw, bool &iscrew)
{
	SPWAW_SNAP_OOB_UELRAW	*uptr, *cptr = NULL;
	DWORD			idx;
	USHORT			lidx = SPWAW_BADIDX;

	ldr = SPWAW_BADIDX;
	crw = SPWAW_BADIDX;
	iscrew = false;

	if (!ptr) return (false);

	uptr = &(ptr->raw[i]);
	if ((uptr->crew != SPWAW_BADIDX) && ((idx = uridx (ptr, uptr->crew)) != SPWAW_BADIDX)) {
		//log ("legacy_ldrcrw[%u] ID=%4.4x, crew=%4.4x\n", i, uptr->RID, uptr->crew);
		cptr = &(ptr->raw[idx]);
		//log ("legacy_ldrcrw[%u] crew=%4.4x, unit=%4.4x\n", i, cptr->RID, cptr->crew);
		if (cptr->crew == SPWAW_BADIDX) return (false);
		if (cptr->crew == uptr->RID) {
			lidx = cptr->leader;
		}
	}
	//log ("legacy_ldrcrw[%u] lidx=%4.4x", i, lidx);
	if (lidx == SPWAW_BADIDX) lidx = uptr->leader;
	//log (", %4.4x", lidx);
	if (lidx == SPWAW_BADIDX) lidx = uptr->RID;
	//log (", %4.4x\n", lidx);

	ldr = lidx;
	crw = (cptr?cptr->RID:SPWAW_BADIDX);
	iscrew = (cptr && (cptr->RID < uptr->RID));

	//if (iscrew) log ("legacy_ldrcrw[%u] ldr=%4.4x, crw=%4.4x, iscrew=%u\n", i, ldr, crw, iscrew);

	return (true);
}

/* Performs the legacy detection of units and crews */
SPWAW_ERROR
snapshot_legacy_ldrcrw_detect (USHORT cnt, SPWAW_SNAP_OOB_RAW *oob)
{
	ULONG	i;
	USHORT	ldridx, crwidx;
	bool	iscrew;

	for (i=0; i<cnt; i++) {
		if (!snapshot_legacy_determine_ldrcrw (&(oob->units), i, ldridx, crwidx, iscrew)) continue;
		if (!iscrew) {
			oob->units.raw[i].dutype = SPWAW_UNIT_TYPE_UNIT;
			if (oob->units.raw[i].leader == SPWAW_BADIDX) oob->units.raw[i].aband = SPWAW_ASTAY;
		} else {
			oob->units.raw[i].dutype = SPWAW_UNIT_TYPE_CREW;
			oob->units.raw[i].aband = SPWAW_ALEFT;
		}
		oob->units.raw[i].leader = ldridx;
		//log ("snapshot_legacy_ldrcrw_detect[%u] type=%s leader=%5.5u crew=%5.5u iscrew=%u aband=%s\n",
		//	i, SPWAW_unittype2str (oob->units.raw[i].dutype),
		//	oob->units.raw[i].leader, oob->units.raw[i].crew, iscrew,
		//	SPWAW_aband2str (oob->units.raw[i].aband));
	}

	return (SPWERR_OK);
}
