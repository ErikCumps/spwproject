/** \file
 * The SPWaW war cabinet - resource handling - plot colors.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"

typedef struct s_PLOT_EXP_MAP {
	SPWAW_EXP	id;
	unsigned long	rid;
} PLOT_EXP_MAP;

#define	EXPMAP(id_,map_)	{ SPWAW_E##id_, RID_PLOT_EXP_##map_ }

static PLOT_EXP_MAP expmap[SPWAW_EXP_CNT] = {
	EXPMAP (CADET,		CAD),
	EXPMAP (GREEN,		GRN),
	EXPMAP (AVERAGE,	AVG),
	EXPMAP (VETERAN,	VET),
	EXPMAP (ELITE,		ELT),
};

static PLOT_EXP_MAP *expmap_cache[SPWAW_EXP_CNT] = { 0 };

QColor *
RES_PLOT_color (SPWAW_EXP id)
{
	PLOT_EXP_MAP	*p;

	if (!(p = expmap_cache[id])) {
		for (int i=0; i<SPWAW_EXP_CNT; i++)
			if (expmap[i].id == id) { p = &(expmap[i]); break; }
		expmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_RGB_BLACK));
	return (RES_color (p->rid));
}

/* ------------------------------------------------------------ */

typedef struct s_PLOT_RANK_MAP {
	SPWAW_RANK	id;
	unsigned long	rid;
} PLOT_RANK_MAP;

#define	RANKMAP(id_)	{ SPWAW_R##id_, RID_PLOT_RNK_##id_ }

static PLOT_RANK_MAP rankmap[SPWAW_RANK_CNT] = {
	RANKMAP (KIA),
	RANKMAP (PVT),
	RANKMAP (CPL),
	RANKMAP (SGT),
	RANKMAP (2LT),
	RANKMAP (1LT),
	RANKMAP (CPT),
	RANKMAP (MAJ),
	RANKMAP (LTC),
	RANKMAP (COL),
	RANKMAP (GEN),
};

static PLOT_RANK_MAP *rankmap_cache[SPWAW_RANK_CNT] = { 0 };

QColor *
RES_PLOT_color (SPWAW_RANK id)
{
	PLOT_RANK_MAP	*p;

	if (!(p = rankmap_cache[id])) {
		for (int i=0; i<SPWAW_RANK_CNT; i++)
			if (rankmap[i].id == id) { p = &(rankmap[i]); break; }
		rankmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_RGB_BLACK));
	return (RES_color (p->rid));
}

/* ------------------------------------------------------------ */

typedef struct s_PLOT_UCLASS_MAP {
	SPWOOB_UCLASS	id;
	unsigned long	rid;
} PLOT_UCLASS_MAP;

#define	UCLASSMAP(id_)	{ SPWOOB_UCLASS_##id_, RID_PLOT_CLASS_##id_ }

static PLOT_UCLASS_MAP uclassmap[SPWOOB_UCLASS_CNT] = {
	UCLASSMAP (INF),
	UCLASSMAP (CAV),
	UCLASSMAP (ARM),
	UCLASSMAP (APC),
	UCLASSMAP (ART),
	UCLASSMAP (AT),
	UCLASSMAP (AA),
	UCLASSMAP (MAR),
	UCLASSMAP (REC),
	UCLASSMAP (CC),
	UCLASSMAP (TRN),
	UCLASSMAP (AIR),
	UCLASSMAP (NVY),
	UCLASSMAP (FRT),
	UCLASSMAP (OTH),
};

static PLOT_UCLASS_MAP *uclassmap_cache[SPWOOB_UCLASS_CNT] = { 0 };

QColor *
RES_PLOT_color (SPWOOB_UCLASS id)
{
	PLOT_UCLASS_MAP	*p;

	if (!(p = uclassmap_cache[id])) {
		for (int i=0; i<SPWOOB_UCLASS_CNT; i++)
			if (uclassmap[i].id == id) { p = &(uclassmap[i]); break; }
		uclassmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_RGB_BLACK));
	return (RES_color (p->rid));
}

