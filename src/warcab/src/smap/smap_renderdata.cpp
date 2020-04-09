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

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.grey, SPWAW_GAME_TYPE_SPWAW, SMAP_HPMC_ID_GREY))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.topo, SPWAW_GAME_TYPE_SPWAW, SMAP_HPMC_ID_TOPO))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.tnor, SPWAW_GAME_TYPE_SPWAW, SMAP_HPMC_ID_NORMAL))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.tjun, SPWAW_GAME_TYPE_SPWAW, SMAP_HPMC_ID_JUNGLE))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_spwaw.tdes, SPWAW_GAME_TYPE_SPWAW, SMAP_HPMC_ID_DESERT))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.grey, SPWAW_GAME_TYPE_WINSPWW2, SMAP_HPMC_ID_GREY))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.topo, SPWAW_GAME_TYPE_WINSPWW2, SMAP_HPMC_ID_TOPO))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.tnor, SPWAW_GAME_TYPE_WINSPWW2, SMAP_HPMC_ID_NORMAL))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.tjun, SPWAW_GAME_TYPE_WINSPWW2, SMAP_HPMC_ID_JUNGLE))
		goto handle_error;

	if (!SMAP_RENDERDATA_HPMC_create (rd.pmc, rd.hpmc_winspww2.tdes, SPWAW_GAME_TYPE_WINSPWW2, SMAP_HPMC_ID_DESERT))
		goto handle_error;

	return (true);

handle_error:
	SMAP_RENDERDATA_destroy (rd);
	return (false);
}

void
SMAP_RENDERDATA_destroy (SMAP_RENDERDATA &rd)
{
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.grey);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.topo);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.tnor);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.tjun);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_spwaw.tdes);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.grey);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.topo);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.tnor);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.tjun);
	SMAP_RENDERDATA_HPMC_destroy (rd.hpmc_winspww2.tdes);
	SMAP_RENDERDATA_PMC_destroy (rd.pmc);
	memset (&rd, 0, sizeof(rd));
}

SMAP_RENDERDATA_HPMC *
SMAP_RENDERDATA_hpmc (SMAP_RENDERDATA &rd, SMAP_HPMC_TYPE type, SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN terrain)
{
	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			{
			switch (type) {
				case SMAP_HPMC_GREY:
				default:
					return (&(rd.hpmc_spwaw.grey));
					break;
				case SMAP_HPMC_TOPO:
					return (&(rd.hpmc_spwaw.topo));
					break;
				case SMAP_HPMC_TERRAIN:
					{
					switch (terrain) {
						case SPWAW_TDESERT:
							return (&(rd.hpmc_spwaw.tdes));
							break;
						case SPWAW_TJUNGLE:
							return (&(rd.hpmc_spwaw.tjun));
							break;
						case SPWAW_TSUMMER:
						case SPWAW_TWINTER:
						case SPWAW_TROUGH:
						case SPWAW_TUNKNOWN:
						default:
							return (&(rd.hpmc_spwaw.tnor));
							break;
						}
					}
					break;
				}
			}
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			{
			switch (type) {
				case SMAP_HPMC_GREY:
				default:
					return (&(rd.hpmc_winspww2.grey));
					break;
				case SMAP_HPMC_TOPO:
					return (&(rd.hpmc_winspww2.topo));
					break;
				case SMAP_HPMC_TERRAIN:
					{
					switch (terrain) {
						case SPWAW_TDESERT:
							return (&(rd.hpmc_winspww2.tdes));
							break;
						case SPWAW_TJUNGLE:
							return (&(rd.hpmc_winspww2.tjun));
							break;
						case SPWAW_TSUMMER:
						case SPWAW_TWINTER:
						case SPWAW_TROUGH:
						case SPWAW_TUNKNOWN:
						default:
							return (&(rd.hpmc_winspww2.tnor));
							break;
						}
					}
					break;
				}
			}
			break;
	}
}