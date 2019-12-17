/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "common/internal.h"

/* Set to 1 to enable dossier search logging */
#define	DSLOG	0

SPWAW_ERROR
dossier_search_back (BIRURR &fr, BIRURR &rr)
{
	BIRURR	nrr;

	SPWAW_BDATE(fr.b->bdate, bdate);
#if DSLOG
	log ("dossier_search_back (fr=[%s:%05u])\n", bdate, fr.i);
#endif	/* DSLOG */

	nrr.b = fr.b; nrr.i = fr.i;
#if DSLOG
	log ("  search_back__ [%s:%05u] src=%05u\n", bdate, nrr.i, nrr.b->ra[nrr.i].src);
#endif	/* DSLOG */

	while ((nrr.b->ra[nrr.i].src != SPWAW_BADIDX) && nrr.b->prev)
	{
		nrr.i = nrr.b->ra[nrr.i].src;
		nrr.b = nrr.b->prev;

		SPWAW_BDATE(nrr.b->bdate, bdate);
#if DSLOG
		log ("  search_back.. [%s:%05u] src=%05u\n", bdate, nrr.i, nrr.b->ra[nrr.i].src);
#endif	/* DSLOG */
	}

	rr = nrr;
	return (SPWERR_OK);
}

static bool
is_empty_filter (BIRURR_FILTER &filter)
{
	bool b = true;
	if (b && filter.lname	) b = false;
	if (b && filter.UID	) b = false;
	if (b && filter.uname	) b = false;
	if (b && filter.rank	) b = false;
	return (b);
}

static bool
filter_match (BIRURR_FILTER &filter, SPWAW_SNAP_OOB_UEL *uel)
{
	bool b = true;
	if (b && filter.lname	&& (strcmp(uel->data.lname, filter.lname) != 0)	) b = false;
	if (b && filter.UID	&& (strcmp(uel->strings.uid, filter.UID) != 0)	) b = false;
	if (b && filter.uname	&& (strcmp(uel->data.uname, filter.uname) != 0)	) b = false;
	if (b && filter.rank	&& (uel->data.rank != *(filter.rank))		) b = false;
	return (b);
}

#if DSLOG
static void
filter_log (BIRURR_FILTER &filter, char *buf, int len)
{
	if (!buf || !len) return;
	memset (buf, 0, len);

	snprintf (buf, len - 1,
		"{lname=\"%s\", UID=\"%s\", uname=\"%s\", rank=\"%s\"}",
		filter.lname	? filter.lname				:"",
		filter.UID	? filter.UID				:"",
		filter.uname	? filter.uname				:"",
		filter.rank	? SPWAW_rank2str(*(filter.rank))	:""
		);
}
#endif	/* DSLOG */

SPWAW_ERROR
dossier_search_back (BIRURR &fr, BIRURR &rr, BIRURR_FILTER &filter)
{
	BIRURR			nrr;
	SPWAW_SNAP_OOB_UEL	*uir;
	
	if (is_empty_filter(filter)) return (dossier_search_back (fr, rr));

	SPWAW_BDATE(fr.b->bdate, bdate);
#if DSLOG
	char	buf[256];
	filter_log (filter, buf, sizeof(buf));
	log ("dossier_search_back (fr=[%s:%05u], filter=%s)\n", bdate, fr.i, buf);
#endif	/* DSLOG */

	nrr.b = fr.b; nrr.i = fr.i; uir = nrr.b->info_sob->pbir_core.uir[nrr.i].snap;
#if DSLOG
	log ("  search_back__ [%s:%05u] src=%05u name=%s\n", bdate, nrr.i, nrr.b->ra[nrr.i].src, uir->data.lname);
#endif	/* DSLOG */

	while ((nrr.b->ra[nrr.i].src != SPWAW_BADIDX) && nrr.b->prev)
	{
		SPWAW_SNAP_OOB_UEL *puir = nrr.b->prev->info_sob->pbir_core.uir[nrr.b->ra[nrr.i].src].snap;
		if (!filter_match (filter, puir)) break;

		nrr.i = nrr.b->ra[nrr.i].src;
		nrr.b = nrr.b->prev;

		SPWAW_BDATE(nrr.b->bdate, bdate);
#if DSLOG
		log ("  search_back.. [%s:%05u] src=%05u name=%s\n", bdate, nrr.i, nrr.b->ra[nrr.i].src, puir->data.lname);
#endif	/* DSLOG */
	}

	rr = nrr;
	return (SPWERR_OK);
}
