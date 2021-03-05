/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_spwoob.h>
#include "spwoob_nations.h"
#include "spwoob_nations_spwaw.h"
#include "spwoob_nations_winspww2.h"
#include "spwoob_nations_winspmbt.h"
#include "common/internal.h"

static const OOBSTRINGS UNKNOWN = { "Unknown", "Unknown nation", "??", "NATION_??" };

/* Convenience macro to build a simple comparable timestamp from a year/month date */
#define	SIMPLE_STAMP(yr_,mo_) ((yr_)*12+((mo_)-1))

const OOBSTRINGS *
lookup_oobstrings (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	int		td = SIMPLE_STAMP(year, month);
	const IDMAP	*idmap;
	const HISTORY	*hist;
	int		i;

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
			idmap = spwaw_idmap;
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			idmap = winspww2_idmap;
			break;
		case SPWAW_GAME_TYPE_WINSPMBT:
			idmap = winspmbt_idmap;
			break;
		case SPWAW_GAME_TYPE_UNKNOWN:
		default:
			ERROR0 ("unsupported game type");
			return (NULL);
			break;
	}

	i = 0;
	while (idmap[i].id != 0) {
		if (idmap[i].id == id) break;
		i++;
	}

	if (idmap[i].id == SPWOOB_UNKNOWN)
		return &UNKNOWN;

	if (year == 0 && month == 0)
		return (&(idmap[i].strings));

#if	HISTACCNATIONS
	if (td < SIMPLE_STAMP(idmap[i].start_year, idmap[i].start_month))
		return &UNKNOWN;
	if (SIMPLE_STAMP(idmap[i].end_year, idmap[i].end_month) <= td)
		return &UNKNOWN;
#else	/* !HISTACCNATIONS */
	if (td < SIMPLE_STAMP(idmap[i].start_year, idmap[i].start_month)) {
		if (idmap[i].history)
			return (&(idmap[i].history->list[0].strings));
		else
			return (&(idmap[i].strings));
	}
	if (SIMPLE_STAMP(idmap[i].end_year, idmap[i].end_month) <= td) {
		if (idmap[i].history)
			return (&(idmap[i].history->list[idmap[i].history->cnt-1].strings));
		else
			return (&(idmap[i].strings));
	}
#endif	/* !HISTACCNATIONS */

	if (idmap[i].history == NULL)
		return (&(idmap[i].strings));

	hist = idmap[i].history; i = 1;
	while ((i<hist->cnt) && (SIMPLE_STAMP(hist->list[i].from_year, hist->list[i].from_month) <= td)) i++;
	return (&(hist->list[i-1].strings));
}

const char *
spwoob_id2people (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2people(gametype, id, 0, 0));
}

const char *
spwoob_id2people (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->people);
}

const char *
spwoob_id2nation (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2nation(gametype, id, 0, 0));
}

const char *
spwoob_id2nation (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->nation);
}

const char *
spwoob_id2prefix (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2prefix(gametype, id, 0, 0));
}

const char *
spwoob_id2prefix (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->prefix);
}

const char *
spwoob_id2flagid (SPWAW_GAME_TYPE gametype, BYTE id)
{
	return (spwoob_id2flagid(gametype, id, 0, 0));
}

const char *
spwoob_id2flagid (SPWAW_GAME_TYPE gametype, BYTE id, int year, int month)
{
	const OOBSTRINGS *s = lookup_oobstrings (gametype, id, year, month);
	if (!s) return (NULL);
	return (s->flagid);
}
