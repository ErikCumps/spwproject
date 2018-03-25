/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "dossier/dossier.h"

#if	0
#include "dossier/dossier_file.h"
#include "snapshot/snapshot.h"
#include "fileio/fileio.h"
#include "utils/textconv.h"
#include "common/internal.h"


#define	ROF(b)	do { if (!b) { rc = SPWERR_FRFAILED; ERRORGOTO ("TC_v2t()", handle_error); } } while (0)

static SPWAW_ERROR
dossier_import_hdr (FILE *src, int dst, long &pos, DOS_HEADER &hdr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	bool		b;

	memset (&hdr, 0, sizeof (hdr));

	b = TC_t2v (src, "dossier.header.magic",	TC_DATA_ASCIIZ, &(hdr.magic), sizeof (hdr.magic)); ROF(b);
	b = TC_t2v (src, "dossier.header.version",	TC_DATA_ULONG, &(hdr.version));	ROF(b);
	b = TC_t2v (src, "dossier.header.name",		TC_DATA_ULONG, &(hdr.name));	ROF(b);
	b = TC_t2v (src, "dossier.header.comment",	TC_DATA_ULONG, &(hdr.comment));	ROF(b);
	b = TC_t2v (src, "dossier.header.oobdir",	TC_DATA_ULONG, &(hdr.oobdir));	ROF(b);
	b = TC_t2v (src, "dossier.header.OOB",		TC_DATA_BYTE, &(hdr.OOB));	ROF(b);
	b = TC_t2v (src, "dossier.header.fcnt",		TC_DATA_USHORT, &(hdr.fcnt));	ROF(b);
	b = TC_t2v (src, "dossier.header.ucnt",		TC_DATA_USHORT, &(hdr.ucnt));	ROF(b);
	b = TC_t2v (src, "dossier.header.bcnt",		TC_DATA_USHORT, &(hdr.bcnt));	ROF(b);
	b = TC_t2v (src, "//dossier.header.blist",	TC_DATA_ULONG, &(hdr.blist));	ROF(b);
	b = TC_t2v (src, "//dossier.header.stab",	TC_DATA_ULONG, &(hdr.stab));	ROF(b);

	pos = bseekget (dst);

	return (SPWERR_OK);

handle_error:
	return (rc);
}
#endif	/* 0 */

SPWAW_ERROR
dossier_import (FILE * /*src*/, int /*dst*/)
{
	// This is basically broken
	return (SPWERR_FAILED);

#if	0
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_HEADER	hdr;

	CNULLARG (src);

	rc = dossier_import_hdr (src, dst, pos, hdr);
	ERRORGOTO ("dossier_export_hdr()", handle_error);

	//rc = dossier_import_blist (src, dst, pos + hdr.blist, hdr.bcnt, hdr.ucnt);
	//ERRORGOTO ("dossier_export_blist()", handle_error);

	//rc = dossier_import_stab (src, dst, pos + hdr.stab);
	//ERRORGOTO ("dossier_export_stab()", handle_error);

	bseekset (dst, pos);
	if (!bwrite (dst, (char *)&hdr, sizeof (hdr))) rc = SPWERR_FWFAILED;
	ERRORGOTO ("bwrite(header)", handle_error);

	return (SPWERR_OK);

handle_error:
	return (rc);
#endif	/* 0 */
}
