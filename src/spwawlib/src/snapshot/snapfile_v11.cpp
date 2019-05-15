/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V11 snapshot.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "snapshot/snapfile_v11.h"
#include "snapshot/snapfile_v10.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "common/types.h"

SPWAW_ERROR
snapshot_load_v11_info_header (int fd, SNAP_INFO *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_INFO_V11	hdr_v11;

	if (!hdr) return (SPWERR_NULLARG);

	if (!bread (fd, (char *)&hdr_v11, sizeof (SNAP_INFO_V11), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot info hdr v11) failed", handle_error);

	/* A V11 snapshot info header only lacks the game type at the end.
	 * The only supported game type for V11 snapshots is the SPWAW_GAME_TYPE_SPWAW,
	 * so a quick copy and fix up is all we need :)
	 */
	memcpy (hdr, &hdr_v11, sizeof (SNAP_INFO_V11));
	hdr->gametype = SPWAW_GAME_TYPE_SPWAW;

handle_error:
	return (rc);
}

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

	/* The V11 snapshot data only lacks the player1 and player2 battle results at the
	 * end of the campaign data, so a quick copy and fix up is all we need :)
	 */
	memcpy (&(snap->cmt), &(snap_v11.cmt), sizeof (SNAP_CMT_V11));
	memcpy (&(snap->b), &(snap_v11.b), sizeof (SNAP_BATTLE_V11));
	memcpy (&(snap->c), &(snap_v11.c), sizeof (SNAP_CAMPAIGN_V11));

	/* We didn't save the battle results, they are unknown */
	snap->c.P1result = BR_UNKNOWN;
	snap->c.P2result = BR_UNKNOWN;

handle_error:
	return (rc);
}

SPWAW_ERROR
snapshot_load_v11_map_data (SBR *sbr, SNAP_MAPDATA *data)
{
	/* The V11 snapshot raw map data is identical to the V10 snapshot raw map data. */
	return (snapshot_load_v10_map_data(sbr,data));
}

SPWAW_ERROR
snapshot_load_v11_oob_header (int fd, SNAP_OOBHDR *hdr)
{
	/* The V11 snapshot OOB data header is identical to the V10 snapshot OOB data header. */
	return (snapshot_load_v10_oob_header(fd,hdr));
}