/** \file
 * The SPWaW war cabinet - strategic map - render data - height colorfield pixmap collection.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata_hpmc.h"
#include <util/util.h>

typedef struct s_COLOR {
	int	red;
	int	blue;
	int	green;
} COLOR;

typedef struct s_COLORFIELDS {
	int	cnt;
	COLOR	*list;
} COLORFIELDS;

static COLOR default_color = { 32, 32, 32 };

static COLOR spwaw_grey_colors[] = {
	{  32,  32,  32 },
	{  48,  48,  48 },
	{  73,  73,  73 },
	{  98,  98,  98 },
	{ 123, 123, 123 },
	{ 148, 148, 148 },
	{ 173, 173, 173 },
	{ 198, 198, 198 },
	{ 224, 224, 224 },
};
static COLORFIELDS spwaw_grey = { ARRAYCOUNT(spwaw_grey_colors), spwaw_grey_colors };

static COLOR winspww2_grey_colors[] = {
	 {  32,  32,  32 },
	 {  48,  48,  48 },
	 {  54,  54,  54 },
	 {  60,  60,  60 },
	 {  66,  66,  66 },
	 {  72,  72,  72 },
	 {  78,  78,  78 },
	 {  84,  84,  84 },
	 {  90,  90,  90 },
	 {  96,  96,  96 },
	 { 102, 102, 102 },
	 { 108, 108, 108 },
	 { 114, 114, 114 },
	 { 120, 120, 120 },
	 { 126, 126, 126 },
	 { 132, 132, 132 },
	 { 138, 138, 138 },
	 { 144, 144, 144 },
	 { 150, 150, 150 },
	 { 156, 156, 156 },
	 { 162, 162, 162 },
	 { 168, 168, 168 },
	 { 174, 174, 174 },
	 { 180, 180, 180 },
	 { 186, 186, 186 },
	 { 192, 192, 192 },
	 { 198, 198, 198 },
	 { 204, 204, 204 },
	 { 210, 210, 210 },
	 { 216, 216, 216 },
	 { 222, 222, 222 },
	 { 228, 228, 228 },
};
static COLORFIELDS winspww2_grey = { ARRAYCOUNT(winspww2_grey_colors), winspww2_grey_colors };

static bool
create_hpmc (SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, COLORFIELDS *cfptr)
{
	QColor color = QColor(default_color.red, default_color.green, default_color.blue);

	if (!cfptr) return (false);

	for (int i=0; i<hpmc.cfcnt; i++) {
		if (i < cfptr->cnt) {
			color = QColor(cfptr->list[i].red, cfptr->list[i].blue, cfptr->list[i].green);
		}
		hpmc.pixmaps[SMAP_hthm2idx(hpmc, i, SMAP_HM_START)] = QPixmap (pmc.width, pmc.height);
		hpmc.pixmaps[SMAP_hthm2idx(hpmc, i, SMAP_HM_START)].fill (color);
	}

	for (int hidx=SMAP_HH_START; hidx<=hpmc.limit; hidx++) {
		for (int midx=SMAP_HM_START; midx<=SMAP_HM_LAST; midx++) {
			int idx = SMAP_hthm2idx(hpmc, hidx, midx);
			if (midx != SMAP_HM_START) {
				hpmc.pixmaps[idx] = hpmc.pixmaps[SMAP_hthm2idx(hpmc, hidx, SMAP_HM_START)];
			}
			hpmc.pixmaps[idx].setAlphaChannel (pmc.hexmask[midx]);
		}
	}

	return (true);
}

bool
SMAP_RENDERDATA_HPMC_create (SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, SPWAW_GAME_TYPE gametype, SPWAW_TERRAIN /*terrain*/)
{
	COLORFIELDS	*cfdef = NULL;

	memset (&hpmc, 0, sizeof(hpmc));

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			cfdef = &spwaw_grey;
			hpmc.limit = SMAP_HH_LAST_SPWAW;
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			cfdef = &winspww2_grey;
			hpmc.limit = SMAP_HH_LAST_WINSPWW2;
			break;
	}

	hpmc.cfcnt = hpmc.limit - SMAP_HH_START + 1;
	hpmc.pmcnt = hpmc.cfcnt * SMAP_HM_CNT;
	hpmc.pixmaps = new QPixmap[hpmc.pmcnt];

	if (!hpmc.pixmaps) goto handle_error;

	return (create_hpmc (pmc, hpmc, cfdef));

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
