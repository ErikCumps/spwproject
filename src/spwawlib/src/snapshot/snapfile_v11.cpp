/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V11 snapshot.
 *
 * Copyright (C) 2018-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/snapfile_v11.h"
#include "snapshot/snapfile_v10.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "common/types.h"
#include "gamefile/spwaw/section08_spwaw.h"

SPWAW_ERROR
snapshot_load_v11_info_header (int fd, SNAP_INFO *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_INFO_V11	hdr_v11;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v11, sizeof (SNAP_INFO_V11), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot info hdr v11) failed", handle_error);

	/* A V11 snapshot info header only lacks the game and save types at the end.
	 * The only supported game type for V11 snapshots is the SPWAW_GAME_TYPE_SPWAW,
	 * the only supported save type for V12 snapshots is the SPWAW_SAVE_TYPE_REGULAR,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, &hdr_v11, sizeof (SNAP_INFO_V11));
	hdr->gametype = SPWAW_GAME_TYPE_SPWAW;
	hdr->savetype = SPWAW_SAVE_TYPE_REGULAR;

handle_error:
	return (rc);
}

#define	copyMETA(name)	snap->meta.##name = snap_v11.meta.##name
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

	/* Copy SNAP_META data */
	copyMETA (title); copyMETA (mapsrc);

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

static void
raw2tfs (SPWAW_TFS &tfs, BYTE tfs1, BYTE tfs2, BYTE tfs3, BYTE tfs4)
{
	SPWAW_TFSBITS1	*tfsbits1 = (SPWAW_TFSBITS1 *)&tfs1;
	SPWAW_TFSBITS2	*tfsbits2 = (SPWAW_TFSBITS2 *)&tfs2;
	SPWAW_TFSBITS3	*tfsbits3 = (SPWAW_TFSBITS3 *)&tfs3;
	SPWAW_TFSBITS4	*tfsbits4 = (SPWAW_TFSBITS4 *)&tfs4;

	memset (&tfs, 0, sizeof(tfs));

	tfs.tfs.field		= tfsbits1->has_field;
	tfs.tfs.slope		= tfsbits1->has_slope;
	tfs.tfs.trees		= tfsbits1->has_trees;
	tfs.tfs.stream		= tfsbits1->has_stream;
	tfs.tfs.building_stone	= tfsbits1->has_bld_stone;
	tfs.tfs.building_wood	= tfsbits1->has_bld_wood;
	tfs.tfs.road1		= tfsbits1->has_road1;
	tfs.tfs.road2		= tfsbits1->has_road2;

	tfs.tfs.bridge_wood	= tfsbits2->has_brg_wood;
	tfs.tfs.bridge_stone	= tfsbits2->has_brg_stone;
	tfs.tfs.swamp		= tfsbits2->has_swamp;
	tfs.tfs.water		= tfsbits2->has_water;
	tfs.tfs.rough		= tfsbits2->has_rough;

	tfs.tfs.railroad	= tfsbits3->has_railroad;
	tfs.tfs.water_shallow	= tfsbits3->has_shwater;
	tfs.tfs.water_deep	= tfsbits3->has_dpwater;
	tfs.tfs.orchard		= tfsbits3->has_orchard;

	tfs.tfs.wall		= tfsbits4->has_wall;
	tfs.tfs.path		= tfsbits4->has_path;
	tfs.tfs.bocage		= tfsbits4->has_bocage;
}

#define	copyMD(name)	data->##name = data_v11.##name

SPWAW_ERROR
snapshot_load_v11_map_data (SBR *sbr, SNAP_MAPDATA *data)
{
	SNAP_MAPDATA_V11	data_v11;
	SPWAW_TFS		tfs;

	CNULLARG (sbr); CNULLARG (data);
	memset (data, 0, sizeof (SNAP_MAPDATA));

	if (sbread (sbr, (char *)&data_v11, sizeof (SNAP_MAPDATA_V11)) != sizeof (SNAP_MAPDATA_V11))
			RWE (SPWERR_FRFAILED, "sbread(v11 raw map data)");

	/* The V11 snapshot unit element lacks the tfs and the tram connection data.
	 * V11 snapshots only support the SPWAW_GAME_TYPE_SPWAW game type, which means:
	 * + the tfs bits must be interpreted as SP:WaW tfs bits
	 * + there are no tramline connections
	 */
	raw2tfs (tfs, data_v11.has_T1, data_v11.has_T2, data_v11.has_T3, data_v11.has_T4);
	data->tfs = tfs.raw;
	data->conn_tram = 0;

	copyMD (height);
	copyMD (has_T1); copyMD (has_T2); copyMD (has_T3); copyMD (has_T4);
	copyMD (conn_road1); copyMD (conn_road2); copyMD (conn_rail);

	return (SPWERR_OK);
}

SPWAW_ERROR
snapshot_load_v11_oob_header (int fd, SNAP_OOBHDR *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_OOBHDR_V11	hdr_v11;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v11, sizeof (SNAP_OOBHDR_V11), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot OOB data hdr v11) failed", handle_error);

	/* The V11 snapshot OOB data header has an unused fstart field. */
	hdr->fcnt  = hdr_v11.fcnt;
	hdr->fpos  = hdr_v11.fpos;
	hdr->fsize = hdr_v11.fsize;
	hdr->fcomp = hdr_v11.fcomp;
	hdr->ucnt  = hdr_v11.ucnt;
	hdr->upos  = hdr_v11.upos;
	hdr->usize = hdr_v11.usize;
	hdr->ucomp = hdr_v11.ucomp;
	hdr->lcnt  = hdr_v11.lcnt;
	hdr->lpos  = hdr_v11.lpos;
	hdr->lsize = hdr_v11.lsize;
	hdr->lcomp = hdr_v11.lcomp;
	hdr->pcnt  = hdr_v11.pcnt;
	hdr->ppos  = hdr_v11.ppos;
	hdr->psize = hdr_v11.psize;
	hdr->pcomp = hdr_v11.pcomp;

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
	copyOU (dutype);
	copyOU (RID); copyOU (FRID); copyOU (FMID); copyOU (FSID);
	copyOU (name);
	copyOU (classID); copyOU (OOB); copyOU (OOBrid);
	copyOU (size); copyOU (cost); copyOU (survive); copyOU (leader);
	copyOU (exp); copyOU (mor); copyOU (sup); copyOU (status); copyOU (entr);
	copyOU (aband);
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
