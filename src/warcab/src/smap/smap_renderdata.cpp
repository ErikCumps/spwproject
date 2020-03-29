/** \file
 * The SPWaW war cabinet - strategic map - render data.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata.h"

bool
SMAP_RENDERDATA_create (SMAP_RENDERDATA &rd, const char *desc, int size)
{
	memset (&rd, 0, sizeof(rd));

	switch (size) {
		case 11:
			rd.width = rd.height = 11; rd.side = 5;
			break;
		case 21:
			rd.width = rd.height = 21; rd.side = 10;
			break;
		default:
			goto handle_error;
			break;
	}
	rd.desc = desc;

	if (!SMAP_RENDERDATA_PMC_create (rd.pmc, size))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.normal, SPWAW_GAME_TYPE_SPWAW, SPWAW_TUNKNOWN))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.desert, SPWAW_GAME_TYPE_SPWAW, SPWAW_TDESERT))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.normal, SPWAW_GAME_TYPE_WINSPWW2, SPWAW_TUNKNOWN))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.desert, SPWAW_GAME_TYPE_WINSPWW2, SPWAW_TDESERT))
		goto handle_error;

	return (true);

handle_error:
	SMAP_RENDERDATA_destroy (rd);
	return (false);
}

void
SMAP_RENDERDATA_destroy (SMAP_RENDERDATA &rd)
{
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.normal);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.desert);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.normal);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.desert);
	SMAP_RENDERDATA_PMC_destroy (rd.pmc);
	memset (&rd, 0, sizeof(rd));
}

SMAP_RENDERDATA_HPMC *
SMAP_RENDERDATA_hpmc (SMAP_RENDERDATA &rd, SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN terrain)
{
	return (&(rd.hpmc_spwaw.normal));
}