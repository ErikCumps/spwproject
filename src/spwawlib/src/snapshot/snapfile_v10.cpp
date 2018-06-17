/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V10 snapshot.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapfile_v10.h"
#include "snapshot/index.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
snapshot_load_v10_info_header (int fd, SNAP_INFO *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_INFO_V10	*hdr_v10;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v10 = safe_malloc (SNAP_INFO_V10); COOMGOTO (hdr_v10, "SNAP_INFO_V10", handle_error);

	if (!bread (fd, (char *)hdr_v10, sizeof (SNAP_INFO_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot info hdr v10) failed", handle_error);

	/* A V10 snapshot info header only lacks the battle type at the end.
	 * The only supported battle type for V10 snapshots is the SPWAW_CAMPAIGN_BATTLE,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, hdr_v10, sizeof (SNAP_INFO_V10));
	hdr->type = SPWAW_CAMPAIGN_BATTLE;

handle_error:
	if (hdr_v10) safe_free (hdr_v10);
	return (rc);
}

#define	getOU(name)	dst->##name = src->##name

static void
snapshot_load_v10_oobu (SNAP_OOB_UEL_V10 *src, SPWAW_SNAP_OOB_UELRAW *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_OOB_UELRAW));

	/* A V10 snapshot unit element lacks the unit type and abandonment status. */
	dst->type = SPWAW_UNIT_TYPE_UNKNOWN;
	dst->aband = SPWAW_ANONE;

	getOU (RID); getOU (FRID); getOU (FMID); getOU (FSID);
	dst->name = STRTAB_getstr (stab, src->name);
	getOU (classID); getOU (OOB); getOU (OOBrid);
	getOU (size); getOU (cost); getOU (survive); getOU (leader);
	getOU (exp); getOU (mor); getOU (sup); getOU (status); getOU (entr);
	getOU (smkdev); getOU (smkammo); getOU (crew);
	getOU (range); getOU (stance_x); getOU (stance_y);
	getOU (loader); getOU (load_cap); getOU (load_cost);
	getOU (radio); getOU (rof); getOU (tgt); getOU (rf); getOU (fc); getOU (iv);
	getOU (swim); getOU (men); getOU (men_ori); getOU (speed); getOU (moves);
	getOU (damage); getOU (movdir); getOU (shtdir); getOU (target); getOU (UTGidx);
//	getOU (SPECIAL_OU); getOU (SPECIAL[0]); getOU (SPECIAL[1]);
//	getOU (SPECIAL[2]); getOU (SPECIAL[3]); getOU (SPECIAL[4]);
}

SPWAW_ERROR
snapshot_load_v10_oobu_list (SBR *sbr, USHORT cnt, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab)
{
	ULONG			i;
	SNAP_OOB_UEL_V10	u;

	for (i=0; i<cnt; i++) {
		if (sbread (sbr, (char *)&u, sizeof (u)) != sizeof (u))
			RWE (SPWERR_FRFAILED, "sbread(unit data)");
		snapshot_load_v10_oobu (&u, &(oob->units.raw[i]), stab);
	}

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
			oob->units.raw[i].type = SPWAW_UNIT_TYPE_UNIT;
			if (oob->units.raw[i].leader == SPWAW_BADIDX) oob->units.raw[i].aband = SPWAW_ASTAY;
		} else {
			oob->units.raw[i].type = SPWAW_UNIT_TYPE_CREW;
			oob->units.raw[i].aband = SPWAW_ALEFT;
		}
		oob->units.raw[i].leader = ldridx;
		//log ("snapshot_legacy_ldrcrw_detect[%u] type=%s leader=%5.5u crew=%5.5u iscrew=%u aband=%s\n",
		//	i, SPWAW_unittype2str (oob->units.raw[i].type),
		//	oob->units.raw[i].leader, oob->units.raw[i].crew, iscrew,
		//	SPWAW_aband2str (oob->units.raw[i].aband));
	}

	return (SPWERR_OK);
}
