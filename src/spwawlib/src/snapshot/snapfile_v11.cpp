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

SPWAW_ERROR
snapshot_load_v11_snap (int fd, SNAP_HEADER *hdr, SNAP *snap)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_V11	*snap_v11;
	CBIO		cbio;

	if (!snap) return (SPWERR_NULLARG);

	snap_v11 = safe_malloc (SNAP_V11); COOMGOTO (snap_v11, "SNAP_V11", handle_error);

	cbio.data = (char *)snap_v11; cbio.size = hdr->snap.size; cbio.comp = &(hdr->snap.comp);
	if (!cbread (fd, cbio, "snapshot game data v11"))
		FAILGOTO (SPWERR_FRFAILED, "cbread(snapshot game data v11) failed", handle_error);


	/* The V11 snapshot data only lacks the player1 and player2 battle results at the
	 * end of the campaign data, so a quick copy and fix up is all we need :)
	 */
	memcpy (&(snap->cmt), &(snap_v11->cmt), sizeof (SNAP_CMT_V11));
	memcpy (&(snap->b), &(snap_v11->b), sizeof (SNAP_BATTLE_V11));
	memcpy (&(snap->c), &(snap_v11->c), sizeof (SNAP_CAMPAIGN_V11));

	/* We didn't save the battle results, they are unknown */
	snap->c.P1result = BR_UNKNOWN;
	snap->c.P2result = BR_UNKNOWN;

handle_error:
	if (snap_v11) safe_free (snap_v11);
	return (rc);
}
