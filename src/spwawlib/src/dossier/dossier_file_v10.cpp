/** \file
 * The SPWaW Library - dossier handling - backwards compatibility with the V10 dossier.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "spwoob/spwoob_list.h"
#include "dossier/dossier_file_v10.h"
#include "fileio/fileio.h"
#include "common/internal.h"

SPWAW_ERROR
dossier_load_v10_header (int fd, DOS_HEADER *hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_HEADER_V10	*hdr_v10;

	if (!hdr) return (SPWERR_NULLARG);

	hdr_v10 = safe_malloc (DOS_HEADER_V10); COOMGOTO (hdr_v10, "DOS_HEADER_V10", handle_error);

	if (!bread (fd, (char *)hdr_v10, sizeof (DOS_HEADER_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(dossier hdr v10)", handle_error);

	/* A V10 dossier header:
	 * + lacks the dossier type
	 * + lacks the dossier game type
	 * + lacks the campaign properties
	 * + but contains player OOB and core formation and unit counts
	 *
	 * V10 dossiers only support the SPWAW_CAMPAIGN_DOSSIER dossier type.
	 * V10 dossiers only support the SPWAW_GAME_TYPE_SPWAW game type.
	 */
	hdr->name	= hdr_v10->name;
	hdr->comment	= hdr_v10->comment;
	hdr->oobdir	= hdr_v10->oobdir;
	hdr->oobdata	= hdr_v10->oobdat;
	hdr->bcnt	= hdr_v10->bcnt;
	hdr->blist	= hdr_v10->blist;
	hdr->stab	= hdr_v10->stab;
	hdr->type	= SPWAW_CAMPAIGN_DOSSIER;
	hdr->gametype	= SPWAW_GAME_TYPE_SPWAW;

	hdr->props.OOB		= (BYTE)(hdr_v10->OOB & 0xFF);
	hdr->props.fcnt		= hdr_v10->fcnt;
	hdr->props.ucnt		= hdr_v10->ucnt;
	hdr->props.start	= SPWAW_TIMESTAMP_EMPTY;
	hdr->props.end		= SPWAW_TIMESTAMP_EMPTY;
	hdr->props.maxbtlcnt	= 0;

handle_error:
	if (hdr_v10) safe_free (hdr_v10);
	return (rc);
}

SPWAW_ERROR
dossier_load_v10_oobdata (int fd, SPWAW_SPWOOB_LIST *oobdata)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SPWOOB		*oobdat = NULL;

	rc = SPWOOB_new (&oobdat);		ERRORGOTO ("SPWOOB_new()", handle_error);
	rc = SPWOOB_load (oobdat, fd);		ERRORGOTO ("spwoob_load()", handle_error);
	rc = SPWOOB_LIST_add (oobdata, oobdat);	ERRORGOTO ("SPWOOB_LIST_add()", handle_error);

handle_error:
	if (oobdat) SPWOOB_free (&oobdat);
	return (rc);
}

SPWAW_ERROR
dossier_load_v10_battle_headers	(int fd, DOS_BHEADER *hdrs, USHORT cnt)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DOS_BHEADER_V10	*hdrs_v10;
	USHORT		i;

	if (!hdrs || !cnt) return (SPWERR_NULLARG);

	hdrs_v10 = safe_nmalloc (DOS_BHEADER_V10, cnt); COOMGOTO (hdrs_v10, "DOS_BHEADER_V10 list", handle_error);

	if (!bread (fd, (char *)hdrs_v10, cnt * sizeof (DOS_BHEADER_V10), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs v10)", handle_error);

	/* A V10 battle header:
	 * + has a slightly different OOB organization
	 * + lacks the battle OOB data index (at the end)
	 * + lacks the battle name symbol (at the end)
	 * + lacks the campaign battle index (at the end)
	 *
	 * V10 dossiers only support a single OOB data, the battle OOB data index must be zero.
	 * V10 dossiers only support the SPWAW_CAMPAIGN_DOSSIER dossier type which doesn't use battle names.
	 * V10 dossiers can only specify an SPWAW_NOBTLIDX (a later override to set the correct btlidx is possible).
	 *
	 * So a quick copy and fix up is all we need :)
	 */
	for (i=0; i<cnt; i++) {
		memcpy (&(hdrs[i]), &(hdrs_v10[i]), sizeof (DOS_BHEADER_V10));
		hdrs[i].OOB_p1 = (BYTE)((hdrs_v10[i].OOB >> 16) & 0xFF);
		hdrs[i].OOB_p2 = (BYTE)((hdrs_v10[i].OOB >>  0) & 0xFF);
		hdrs[i].oobdat = 0;
		hdrs[i].name = BADSTRIDX;
		hdrs[i].btlidx = SPWAW_NOBTLIDX;
	}

handle_error:
	if (hdrs_v10) safe_free (hdrs_v10);
	return (rc);
}
