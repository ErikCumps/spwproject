/** \file
 * The SPWaW war cabinet - strategic map - render data - height colorfield pixmap collection.
 *
 * Copyright (C) 2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "smap_renderdata_hpmc.h"
#include <util/util.h>

/*** definitions ***/
typedef struct s_COLOR {
	int	red;
	int	blue;
	int	green;
} COLOR;

static COLOR default_color = { 32, 32, 32 };

typedef struct s_COLORFIELDS {
	int	cnt;
	COLOR	*list;
	int	saturation;
	int	brightness;
} COLORFIELDS;

typedef struct s_CFDEF_ITEM {
	SMAP_HPMC_ID	id;
	COLORFIELDS	*def;
} CFDEF_ITEM;



/*******************************/
/***   SP:WaW color fields   ***/
/*******************************/
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
static COLORFIELDS spwaw_grey = { ARRAYCOUNT(spwaw_grey_colors), spwaw_grey_colors, 100, 100 };

static COLOR spwaw_topo_colors[] = {
	{  34,153,113 },
	{  22,186, 55 },
	{  59,222,  4 },
	{ 138,237,  0 },
	{ 221,250,  0 },
	{ 250,226,  7 },
	{ 242,185, 15 },
	{ 224,141, 16 },
	{ 194, 91,  8 },
	{ 168, 56,  0 },
};
static COLORFIELDS spwaw_topo = { ARRAYCOUNT(spwaw_topo_colors), spwaw_topo_colors, 25, 66 };

static COLOR spwaw_normal_colors[] = {
	{ 112,153, 89 },
	{ 163,199,125 },
	{ 214,237,154 },
	{ 240,228,141 },
	{ 230,186,133 },
	{ 201,149,127 },
	{ 214,173,148 },
	{ 245,225,196 },
	{ 250,241,227 },
	{ 252,252,252 },
};
static COLORFIELDS spwaw_normal = { ARRAYCOUNT(spwaw_normal_colors), spwaw_normal_colors, 50, 66 };

static COLOR spwaw_jungle_colors[] = {
	{   0, 79,  0 },
	{   3, 99,  0 },
	{  24,122,  0 },
	{  55,138,  0 },
	{ 150,194, 68 },
	{ 223,247, 62 },
	{ 235,172,  0 },
	{ 161, 94,  0 },
	{ 156,156,156 },
	{ 252,252,252 },
};
static COLORFIELDS spwaw_jungle = { ARRAYCOUNT(spwaw_jungle_colors), spwaw_jungle_colors, 25, 66 };

static COLOR spwaw_desert_colors[] = {
	{  79, 79, 79 },
	{  97, 86, 81 },
	{ 117, 91, 79 },
	{ 138, 91, 69 },
	{ 161,115, 80 },
	{ 181,140, 91 },
	{ 204,168,102 },
	{ 222,183,111 },
	{ 240,198,120 },
	{ 255,210,128 },
};
static COLORFIELDS spwaw_desert = { ARRAYCOUNT(spwaw_desert_colors), spwaw_desert_colors, 50, 100 };

static CFDEF_ITEM spwaw_cfdef_items[] = {
	{ SMAP_HPMC_ID_GREY,	&spwaw_grey	},
	{ SMAP_HPMC_ID_TOPO,	&spwaw_topo	},
	{ SMAP_HPMC_ID_NORMAL,	&spwaw_normal	},
	{ SMAP_HPMC_ID_JUNGLE,	&spwaw_jungle	},
	{ SMAP_HPMC_ID_DESERT,	&spwaw_desert	},
	{ SMAP_HPMC_UNKNOWN,	NULL		},
};



/*******************************/
/***   winSPWW2 color fields   ***/
/*******************************/
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
static COLORFIELDS winspww2_grey = { ARRAYCOUNT(winspww2_grey_colors), winspww2_grey_colors, 100, 100 };

static COLOR winspww2_topo_colors[] = {
	{  34,153,113 },
	{  32,161,101 },
	{  29,171, 86 },
	{  25,181, 67 },
	{  21,191, 47 },
	{  16,201, 19 },
	{  34,212, 11 },
	{  59,222,  4 },
	{  80,230,  0 },
	{ 104,232,  0 },
	{ 125,235,  0 },
	{ 148,240,  0 },
	{ 170,242,  0 },
	{ 196,245,  0 },
	{ 221,250,  0 },
	{ 244,252,  0 },
	{ 252,244,  3 },
	{ 250,234,  5 },
	{ 247,219,  7 },
	{ 245,206, 10 },
	{ 242,192, 12 },
	{ 242,185, 15 },
	{ 240,173, 17 },
	{ 237,161, 19 },
	{ 230,149, 18 },
	{ 222,132, 16 },
	{ 212,119, 13 },
	{ 204,104, 10 },
	{ 194, 91,  8 },
	{ 186, 78,  6 },
	{ 176, 67,  4 },
	{ 168, 56,  0 },
};
static COLORFIELDS winspww2_topo = { ARRAYCOUNT(winspww2_topo_colors), winspww2_topo_colors, 25, 66 };

static COLOR winspww2_normal_colors[] = {
	{ 112,153, 89 },
	{ 127,166, 99 },
	{ 140,179,109 },
	{ 155,191,119 },
	{ 172,207,132 },
	{ 187,219,143 },
	{ 202,232,153 },
	{ 214,237,154 },
	{ 223,237,149 },
	{ 233,237,147 },
	{ 240,233,144 },
	{ 240,221,139 },
	{ 240,208,134 },
	{ 237,197,133 },
	{ 230,186,133 },
	{ 222,175,131 },
	{ 214,165,131 },
	{ 204,152,126 },
	{ 196,145,126 },
	{ 196,145,128 },
	{ 204,158,137 },
	{ 214,173,148 },
	{ 222,187,160 },
	{ 232,203,174 },
	{ 240,217,187 },
	{ 247,230,203 },
	{ 247,233,210 },
	{ 250,238,220 },
	{ 250,241,227 },
	{ 252,247,237 },
	{ 252,250,245 },
	{ 252,252,252 },
};
static COLORFIELDS winspww2_normal = { ARRAYCOUNT(winspww2_normal_colors), winspww2_normal_colors, 50, 66 };

static COLOR winspww2_jungle_colors[] = {
	{   0, 79,  0 },
	{   0, 84,  0 },
	{   0, 92,  0 },
	{   0, 97,  0 },
	{   5,105,  0 },
	{  13,110,  0 },
	{  20,117,  0 },
	{  24,122,  0 },
	{  27,125,  0 },
	{  30,128,  0 },
	{  44,133,  0 },
	{  68,140,  0 },
	{  90,145,  0 },
	{ 118,166, 22 },
	{ 150,194, 68 },
	{ 186,222,124 },
	{ 207,240,137 },
	{ 213,245, 86 },
	{ 235,250, 32 },
	{ 250,237,  0 },
	{ 242,206,  0 },
	{ 235,172,  0 },
	{ 222,144,  0 },
	{ 196,121,  0 },
	{ 173,104,  0 },
	{ 156, 95,  9 },
	{ 156,115, 59 },
	{ 156,137,110 },
	{ 156,156,156 },
	{ 189,189,189 },
	{ 219,219,219 },
	{ 252,252,252 },
};
static COLORFIELDS winspww2_jungle = { ARRAYCOUNT(winspww2_jungle_colors), winspww2_jungle_colors, 25, 66 };

static COLOR winspww2_desert_colors[] = {
	{  79, 79, 79 },
	{  84, 82, 81 },
	{  89, 84, 81 },
	{  94, 85, 81 },
	{  99, 87, 81 },
	{ 107, 90, 81 },
	{ 112, 91, 81 },
	{ 117, 91, 79 },
	{ 122, 91, 76 },
	{ 128, 90, 73 },
	{ 135, 91, 70 },
	{ 140, 94, 70 },
	{ 145, 99, 73 },
	{ 153,107, 77 },
	{ 161,115, 80 },
	{ 166,122, 83 },
	{ 173,130, 87 },
	{ 179,137, 89 },
	{ 186,146, 93 },
	{ 191,153, 96 },
	{ 199,162, 99 },
	{ 204,168,102 },
	{ 209,173,105 },
	{ 214,177,107 },
	{ 219,181,110 },
	{ 224,185,112 },
	{ 230,189,115 },
	{ 235,194,117 },
	{ 240,198,120 },
	{ 242,200,121 },
	{ 247,204,124 },
	{ 255,210,128 },
};
static COLORFIELDS winspww2_desert = { ARRAYCOUNT(winspww2_desert_colors), winspww2_desert_colors, 50, 100 };

static CFDEF_ITEM winspww2_cfdef_items[] = {
	{ SMAP_HPMC_ID_GREY,	&winspww2_grey		},
	{ SMAP_HPMC_ID_TOPO,	&winspww2_topo		},
	{ SMAP_HPMC_ID_NORMAL,	&winspww2_normal	},
	{ SMAP_HPMC_ID_JUNGLE,	&winspww2_jungle	},
	{ SMAP_HPMC_ID_DESERT,	&winspww2_desert	},
	{ SMAP_HPMC_UNKNOWN,	NULL			},
};


/*** code ***/

static inline COLORFIELDS *
lookup_cfdef (CFDEF_ITEM *list, SMAP_HPMC_ID id)
{
	int	i = 0;

	while (list[i].id != SMAP_HPMC_UNKNOWN) {
		if (list[i].id == id) return (list[i].def);
		i++;
	}
	return (NULL);
}

static inline void
patch_color (QColor &color, int saturation, int brightness)
{
	if ((saturation == 100) && (brightness == 100)) return;

	QColor hsv = color.toHsv();

	int	h, s, v, a;
	hsv.getHsv (&h, &s, &v, &a);
	if (saturation != 100) s = (s * saturation) / 100;
	if (brightness != 100) v = (v * brightness) / 100;
	hsv.setHsv (h, s, v, a);

	color = hsv.toRgb();
}

static bool
create_hpmc (SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, COLORFIELDS *cfdef)
{
	QColor color = QColor(default_color.red, default_color.green, default_color.blue);

	for (int i=0; i<hpmc.cfcnt; i++) {
		if (cfdef && (i < cfdef->cnt)) {
			color = QColor(cfdef->list[i].red, cfdef->list[i].blue, cfdef->list[i].green);
			patch_color (color, cfdef->saturation, cfdef->brightness);
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

static bool
SMAP_RENDERDATA_HPMC_create_core (SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, SMAP_HH limit, CFDEF_ITEM *cflist, SMAP_HPMC_ID id)
{
	memset (&hpmc, 0, sizeof(hpmc));

	hpmc.limit = limit;
	hpmc.cfcnt = hpmc.limit - SMAP_HH_START + 1;
	hpmc.pmcnt = hpmc.cfcnt * SMAP_HM_CNT;
	hpmc.pixmaps = new QPixmap[hpmc.pmcnt];

	if (!hpmc.pixmaps) goto handle_error;

	return (create_hpmc (pmc, hpmc, lookup_cfdef(cflist, id)));

handle_error:
	SMAP_RENDERDATA_HPMC_destroy (hpmc);
	return (false);
}

bool
SMAP_RENDERDATA_HPMC_create (SMAP_RENDERDATA_PMC &pmc, SMAP_RENDERDATA_HPMC &hpmc, SPWAW_GAME_TYPE gametype, SMAP_HPMC_ID id)
{
	memset (&hpmc, 0, sizeof(hpmc));

	switch (gametype) {
		case SPWAW_GAME_TYPE_SPWAW:
		default:
			return (SMAP_RENDERDATA_HPMC_create_core (pmc, hpmc, SMAP_HH_LAST_SPWAW, spwaw_cfdef_items, id));
			break;
		case SPWAW_GAME_TYPE_WINSPWW2:
			return (SMAP_RENDERDATA_HPMC_create_core (pmc, hpmc, SMAP_HH_LAST_WINSPWW2, winspww2_cfdef_items, id));
			break;
	}
}

void
SMAP_RENDERDATA_HPMC_destroy (SMAP_RENDERDATA_HPMC &hpmc)
{
	if (hpmc.pixmaps) delete[] hpmc.pixmaps;
	memset (&hpmc, 0, sizeof(hpmc));
}
