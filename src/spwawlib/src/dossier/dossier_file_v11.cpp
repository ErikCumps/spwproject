/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V11 dossier.
 *
 * Copyright (C) 2019-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier_file_v11.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v11_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V11	*hdr_v11;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v11 = safe_malloc (DOS_HEADER_V11); COOMGOTO (hdr_v11, "DOS_HEADER_V11", handle_error);

	if (!bread (fd, (char *)hdr_v11, sizeof (DOS_HEADER_V11), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr v11)", handle_error);

	/* A V11 dossier header:
	 * + lacks the savedir
	 * + lacks the dossier game type
	 * + lacks the campaign properties
	 * + lacks the UHT data
	 * + lacks the savegame tracking info
	 * + but contains player OOB and core formation and unit counts
	 *
	 * V11 dossiers only support the SPWAW_GAME_TYPE_SPWAW game type.
	 * V11 dossiers only support the SPWAW_SAVE_TYPE_REGULAR save type.
	 * V11 dossiers do not support savegame tracking.
	 */
	hdr->name		= hdr_v11->name;
	hdr->comment		= hdr_v11->comment;
	hdr->savedir		= BADSTRIDX;
	hdr->oobdir		= hdr_v11->oobdir;
	hdr->oobdata		= hdr_v11->oobdata;
	hdr->bcnt		= hdr_v11->bcnt;
	hdr->blist		= hdr_v11->blist;
	hdr->stab		= hdr_v11->stab;
	hdr->type		= hdr_v11->type;
	hdr->gametype		= SPWAW_GAME_TYPE_SPWAW;

	hdr->props.OOB		= (BYTE)(hdr_v11->OOB & 0xFF);
	hdr->props.start	= SPWAW_TIMESTAMP_EMPTY;
	hdr->props.end		= SPWAW_TIMESTAMP_EMPTY;
	hdr->props.maxbtlcnt	= 0;
	hdr->props.ifcnt	= hdr_v11->fcnt;
	hdr->props.iucnt	= hdr_v11->ucnt;
	hdr->props.cfcnt	= hdr_v11->fcnt;
	hdr->props.cucnt	= hdr_v11->ucnt;

	hdr->uht		= 0;

	hdr->tracking.gametype	= SPWAW_GAME_TYPE_UNKNOWN;
	hdr->tracking.savetype	= SPWAW_SAVE_TYPE_UNKNOWN;
	hdr->tracking.oobdir	= BADSTRIDX;
	hdr->tracking.path	= BADSTRIDX;
	hdr->tracking.base	= BADSTRIDX;
	hdr->tracking.filename	= BADSTRIDX;
	hdr->tracking.filedate	= 0;

handle_error:
	if (hdr_v11) safe_free (hdr_v11);
	return (rc);
}

SPWAW_ERROR
dossier_load_v11_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V11	*hdrs_v11;
	USHORT		i;

	if (!hdrs || !cnt) return (SPWERR_NULLARG);

	hdrs_v11 = safe_nmalloc (DOS_BHEADER_V11, cnt); COOMGOTO (hdrs_v11, "DOS_BHEADER_V11 list", handle_error);

	if (!bread (fd, (char *)hdrs_v11, cnt * sizeof (DOS_BHEADER_V11), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v11)", handle_error);

	/* A V11 battle header:
	 * + lacks the campaign battle index (at the end)
	 * + lacks the battle unit reassignment list element count (at the end)
	 * + lacks the battle map data (at the end)
	 * + lacks the battle location display name (at the end)
	 *
	 * V11 dossiers can only specify an SPWAW_NOBTLIDX (a later override to set the correct btlidx is possible).
	 * For V11 dossiers the RA info is no longer loaded.
	 * For V11 dossiers there is no separately saved battle map data.
	 * For V11 dossiers the battle location display name is not stored.
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	for (i=0; i<cnt; i++) {
		memcpy (&(hdrs[i]), &(hdrs_v11[i]), sizeof (DOS_BHEADER_V11));
		hdrs[i].btlidx = SPWAW_NOBTLIDX;
		hdrs[i].racnt = 0;
		hdrs[i].map.width = hdrs[i].map.height = 0;
		hdrs[i].map.raw.data = hdrs[i].map.raw.size = hdrs[i].map.raw.comp = 0;
		hdrs[i].map.map.data = hdrs[i].map.map.size = hdrs[i].map.map.comp = 0;
		hdrs[i].location = BADSTRIDX;
	}

handle_error:
	if (hdrs_v11) safe_free (hdrs_v11);
	return (rc);
}
