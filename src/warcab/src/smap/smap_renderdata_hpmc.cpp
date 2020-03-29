/** \file
 * The SPWaW war cabinet - strategic map - render data - height colorfield pixmap collection.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata_hpmc.h"
#include "smap_renderdata_xpm.h"

/*! Height colorfield XPM data for hexes of size 11x11 pixels */
extern SMAP_RENDERDATA_HCFXPMS hcfxpms_11x11;

/*! Height colorfield XPM data for hexes of size 21x21 pixels */
extern SMAP_RENDERDATA_HCFXPMS hcfxpms_21x21;

bool
SMAP_RENDERDATA_HPMC_create (SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN terrain)
{
	SMAP_RENDERDATA_HCFXPMS	*hcfxpms;

	memset (&hpmc, 0, sizeof(hpmc));

	switch (pmc.width) {
		case 11:
			hcfxpms = &hcfxpms_11x11;
			break;
		case 21:
			hcfxpms = &hcfxpms_21x21;
			break;
		default:
			return (false);
			break;
	}

	hpmc.limit = SMAP_HH_LAST_SPWAW;
	hpmc.cnt = (hpmc.limit - SMAP_HH_START + 1) * SMAP_HM_CNT;
	hpmc.pixmaps = new QPixmap[hpmc.cnt];

	if (!hpmc.pixmaps) goto handle_error;

	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_NEG, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->hneg;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_000, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h000;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_005, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h005;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_010, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h010;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_015, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h015;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_020, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h020;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_025, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h025;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_030, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h030;
	hpmc.pixmaps[SMAP_hthm2idx(SMAP_HH_035, SMAP_HM_START, SMAP_HH_LAST_SPWAW)] = hcfxpms->h035;
	// FIXME: winSPWW2 has a much greater range of heights!

	for (int hidx=SMAP_HH_START; hidx<=SMAP_HH_LAST_SPWAW; hidx++) {
		for (int midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
			int idx = SMAP_hthm2idx(hidx, midx, SMAP_HH_LAST_SPWAW);
			if (midx != SMAP_HM_START) {
				hpmc.pixmaps[idx] = hpmc.pixmaps[SMAP_hthm2idx(hidx, SMAP_HM_START, SMAP_HH_LAST_SPWAW)];
			}
			hpmc.pixmaps[idx].setAlphaChannel (pmc.hexmask[midx]);
		}
	}

	return (true);

handle_error:
	SMAP_RENDERDATA_HPMC_destroy (hpmc);
	return (false);
}

void
SMAP_RENDERDATA_HPMC_destroy (SMAP_RENDERDATA_HPMC &hpmc)
{
	if (hpmc.pixmaps) delete[] hpmc.pixmaps;
	memset (&hpmc, 0, sizeof(hpmc));
}
