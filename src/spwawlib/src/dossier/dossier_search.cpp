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

SPWAW_ERROR
dossier_search_back (SPWAW_BATTLE *fb, USHORT fi, BIRURR *rr)
{
	CNULLARG (fb); CNULLARG (rr);

	SPWAW_BDATE(fb->bdate, bdate);
	//log ("dossier_search_back (fb=[%s], fi=%05u)\n", bdate, fi);

	rr->b = fb; rr->i = fi;
	//log ("  search_back__ [%s:%05u] src=%05u\n", bdate, rr->i, rr->b->ra[rr->i].src);

	while (	(rr->b->ra[rr->i].src != SPWAW_BADIDX)
		&& rr->b->prev)
	{
		rr->i = rr->b->ra[rr->i].src;
		rr->b = rr->b->prev;

		SPWAW_BDATE(rr->b->bdate, bdate);
		//log ("  search_back.. [%s:%05u] src=%05u\n", bdate, rr->i, rr->b->ra[rr->i].src);
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_search_back (SPWAW_BATTLE *fb, USHORT fi, char *name, BIRURR *rr)
{
	CNULLARG (fb); CNULLARG (name); CNULLARG (rr);

	SPWAW_BDATE(fb->bdate, bdate);
	//log ("dossier_search_back (fb=[%s], fi=%05u, name=\"%s\")\n", bdate, fi, name);

	rr->b = fb; rr->i = fi;
	//log ("  search_back__ [%s:%05u] src=%05u name=%s\n", bdate, rr->i, rr->b->ra[rr->i].src, rr->b->info_sob->pbir_core.uir[rr->i].snap->data.lname);

	while (	(rr->b->ra[rr->i].src != SPWAW_BADIDX)
		&& rr->b->prev
		&& strcmp(rr->b->prev->info_sob->pbir_core.uir[rr->b->ra[rr->i].src].snap->data.lname, name) == 0)
	{
		rr->i = rr->b->ra[rr->i].src;
		rr->b = rr->b->prev;

		SPWAW_BDATE(rr->b->bdate, bdate);
		//log ("  search_back.. [%s:%05u] src=%05u name=%s\n", bdate, rr->i, rr->b->ra[rr->i].src, rr->b->info_sob->pbir_core.uir[rr->i].snap->data.lname);
	}

	return (SPWERR_OK);
}
