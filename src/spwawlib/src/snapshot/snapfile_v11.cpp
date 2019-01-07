/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V11 snapshot.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/snapfile_v11.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "common/types.h"

#define	copyCMT(name)	snap->cmt.##name = snap_v11.cmt.##name
#define	copyB(name)	snap->b.##name = snap_v11.b.##name
#define	copyBVH(i,name)	snap->b.vhex[i].##name = snap_v11.b.vhex[i].##name
#define	copyC(name)	snap->c.##name = snap_v11.c.##name
SPWAW_ERROR
snapshot_load_v11_snap (int fd, SNAP_HEADER *hdr, SNAP *snap)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_V11	snap_v11;
	CBIO		cbio;

	if (!snap) return (SPWERR_NULLARG);

	cbio.data = (char *)&snap_v11; cbio.size = hdr->snap.size; cbio.comp = &(hdr->snap.comp);
	if (!cbread (fd, cbio, "snapshot game data v11"))
		FAILGOTO (SPWERR_FRFAILED, "cbread(snapshot game data v11) failed", handle_error);

	/* The V11 snapshot data storage uses data types with different widths. */

	/* Copy SNAP_CMT data */
	copyCMT (title); copyCMT (mapsrc);

	/* Copy SNAP_BATTLE data */
	copyB (year); copyB (month); copyB (day); copyB (hour); copyB (location);
	copyB (terrain); copyB (weather); copyB (visibility); copyB (turn); copyB (turn_max);
	copyB (OOB_p1); copyB (OOB_p2); copyB (OOB_p3); copyB (OOB_p4);
	copyB (miss_p1); copyB (miss_p2); copyB (credit);

	for (int i=0; i<SNAP_VHEXCNT; i++) {
		copyBVH (i, x); copyBVH (i, y);
		copyBVH (i, value); copyBVH (i, owner);
	}

	/* Copy SNAP_CAMPAIGN data */
	copyC (campaign); copyC (start_year); copyC (start_month); copyC (end_year); copyC (end_month);
	copyC (battles); copyC (majvics); copyC (minvics); copyC (battles_max);
	copyC (P1BLmen); copyC (P1BLart); copyC (P1BLsoft); copyC (P1BLapc); copyC (P1BLafv); copyC (P1BLgliders); copyC (P1BLair);
	copyC (P2BLmen); copyC (P2BLart); copyC (P2BLsoft); copyC (P2BLapc); copyC (P2BLafv); copyC (P2BLgliders); copyC (P2BLair);
	copyC (P1TLmen); copyC (P1TLart); copyC (P1TLsoft); copyC (P1TLapc); copyC (P1TLafv); copyC (P1TLgliders); copyC (P1TLair);
	copyC (P2TLmen); copyC (P2TLart); copyC (P2TLsoft); copyC (P2TLapc); copyC (P2TLafv); copyC (P2TLgliders); copyC (P2TLair);
	copyC (busy); copyC (P1score); copyC (P2score);

	/* The V11 snapshot data also lacks the player1 and player2 battle results
	 * at the end of the campaign data. Because we didn't save the battle results,
	 * they are unknown and are set as such. */
	snap->c.P1result = BR_UNKNOWN;
	snap->c.P2result = BR_UNKNOWN;

handle_error:
	return (rc);
}

#define	copyOF(name)	fel->##name = fel_v11.##name

SPWAW_ERROR
snapshot_load_v11_oob_fel (SBR *sbr, SNAP_OOB_FEL *fel)
{
	SNAP_OOB_FEL_V11	fel_v11;

	CNULLARG (sbr); CNULLARG (fel);

	if (sbread (sbr, (char *)&fel_v11, sizeof (SNAP_OOB_FEL_V11)) != sizeof (SNAP_OOB_FEL_V11))
			RWE (SPWERR_FRFAILED, "sbread(v11 unit data)");

	/* The V11 snapshot data storage uses data types with different widths. */
	copyOF (RID); copyOF (FID);
	copyOF (name);
	copyOF (leader); copyOF (hcmd); copyOF (OOBrid); copyOF (status);

	return (SPWERR_OK);
}

#define	copyOU(name)	uel->##name = uel_v11.##name

SPWAW_ERROR
snapshot_load_v11_oob_uel (SBR *sbr, SNAP_OOB_UEL *uel)
{
	SNAP_OOB_UEL_V11	uel_v11;

	CNULLARG (sbr); CNULLARG (uel);

	if (sbread (sbr, (char *)&uel_v11, sizeof (SNAP_OOB_UEL_V11)) != sizeof (SNAP_OOB_UEL_V11))
			RWE (SPWERR_FRFAILED, "sbread(v11 unit data)");

	/* The V11 snapshot data storage uses data types with different widths. */
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

#define	copyOL(name)	lel->##name = lel_v11.##name

SPWAW_ERROR
snapshot_load_v11_oob_lel (SBR *sbr, SNAP_OOB_LEL *lel)
{
	SNAP_OOB_LEL_V11	lel_v11;

	CNULLARG (sbr); CNULLARG (lel);

	if (sbread (sbr, (char *)&lel_v11, sizeof (SNAP_OOB_LEL_V11)) != sizeof (SNAP_OOB_LEL_V11))
			RWE (SPWERR_FRFAILED, "sbread(v11 unit data)");

	/* The V11 snapshot data storage uses data types with different widths. */
	copyOL (RID); copyOL (URID);
	copyOL (name);
	copyOL (rank); copyOL (ral); copyOL (inf); copyOL (art); copyOL (arm);
	copyOL (kills); copyOL (status);

	return (SPWERR_OK);
}
