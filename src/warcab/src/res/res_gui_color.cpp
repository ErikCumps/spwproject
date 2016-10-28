/** \file
 * The SPWaW war cabinet - resource handling - GUI colors.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"

typedef struct s_GUI_RANK_MAP {
	SPWAW_RANK	id;
	unsigned long	rid;
} GUI_RANK_MAP;

#define	RANKMAP(id_)	{ SPWAW_R##id_, RID_GMRC_RNK_##id_ }

static GUI_RANK_MAP rankmap[SPWAW_RANK_CNT] = {
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

static GUI_RANK_MAP *rankmap_cache[SPWAW_RANK_CNT] = { 0 };

QColor *
RES_GUI_color (SPWAW_RANK id)
{
	GUI_RANK_MAP	*p;

	if (!(p = rankmap_cache[id])) {
		for (int i=0; i<SPWAW_RANK_CNT; i++)
			if (rankmap[i].id == id) { p = &(rankmap[i]); break; }
		rankmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_GMRC_BG_DEFAULT));
	return (RES_color (p->rid));
}

/* ------------------------------------------------------------ */

typedef struct s_GUI_USTATUS_MAP {
	SPWAW_USTATUS	id;
	unsigned long	rid;
} GUI_USTATUS_MAP;

#define	USTATUSMAP(id_,map_)	{ SPWAW_U##id_, RID_GMRC_US_##map_ }

static GUI_USTATUS_MAP ustatusmap[SPWAW_USTATUS_CNT] = {
	USTATUSMAP (READY,		READY),
	USTATUSMAP (BUTTONED,		BTTN),
	USTATUSMAP (PINNED,		PIN),
	USTATUSMAP (IMMOBILIZED,	IMMBL),
	USTATUSMAP (RETREATING,		ROUT),
	USTATUSMAP (ROUTED,		ROUT),
	USTATUSMAP (ABANDONED,		ABAND),
	USTATUSMAP (DESTROYED,		DSTRY),
	USTATUSMAP (BURNING,		DSTRY),
	USTATUSMAP (PASSENGER,		PSGR),
};

static GUI_USTATUS_MAP *ustatusmap_cache[SPWAW_USTATUS_CNT] = { 0 };

QColor *
RES_GUI_color (SPWAW_USTATUS id)
{
	GUI_USTATUS_MAP	*p;

	if (!(p = ustatusmap_cache[id])) {
		for (int i=0; i<SPWAW_USTATUS_CNT; i++)
			if (ustatusmap[i].id == id) { p = &(ustatusmap[i]); break; }
		ustatusmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_GMRC_BG_DEFAULT));
	return (RES_color (p->rid));
}

/* ------------------------------------------------------------ */

typedef struct s_GUI_ENTR_MAP {
	SPWAW_ENTR	id;
	unsigned long	rid;
} GUI_ENTR_MAP;

#define	ENTRMAP(id_)	{ SPWAW_ENTR##id_, RID_GMRC_ENTR_##id_ }

static GUI_ENTR_MAP entrmap[SPWAW_ENTR_CNT] = {
	ENTRMAP (NONE),
	ENTRMAP (COVER),
	ENTRMAP (DUGIN),
};

static GUI_ENTR_MAP *entrmap_cache[SPWAW_ENTR_CNT] = { 0 };

QColor *
RES_GUI_color (SPWAW_ENTR id)
{
	GUI_ENTR_MAP	*p;

	if (!(p = entrmap_cache[id])) {
		for (int i=0; i<SPWAW_ENTR_CNT; i++)
			if (entrmap[i].id == id) { p = &(entrmap[i]); break; }
		entrmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_GMRC_BG_DEFAULT));
	return (RES_color (p->rid));
}

/* ------------------------------------------------------------ */

typedef struct s_GUI_EXP_MAP {
	SPWAW_EXP	id;
	unsigned long	rid;
} GUI_EXP_MAP;

#define	EXPMAP(id_,map_)	{ SPWAW_E##id_, RID_GMRC_EXP_##map_ }

static GUI_EXP_MAP expmap[SPWAW_EXP_CNT] = {
	EXPMAP (CADET,		CAD),
	EXPMAP (GREEN,		GRN),
	EXPMAP (AVERAGE,	AVG),
	EXPMAP (VETERAN,	VET),
	EXPMAP (ELITE,		ELT),
};

static GUI_EXP_MAP *expmap_cache[SPWAW_EXP_CNT] = { 0 };

QColor *
RES_GUI_color (SPWAW_EXP id)
{
	GUI_EXP_MAP	*p;

	if (!(p = expmap_cache[id])) {
		for (int i=0; i<SPWAW_EXP_CNT; i++)
			if (expmap[i].id == id) { p = &(expmap[i]); break; }
		expmap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_GMRC_BG_DEFAULT));
	return (RES_color (p->rid));
}

/* ------------------------------------------------------------ */

typedef struct s_GUI_FTYPE_MAP {
	SPWOOB_FTYPE	id;
	unsigned long	rid;
} GUI_FTYPE_MAP;

#define	FTYPEMAP(id_,map_)	{ SPWOOB_FTYPE_##id_, RID_GMOC_FT_##map_ }

static GUI_FTYPE_MAP ftypemap[SPWOOB_FTYPE_CNT] = {
	FTYPEMAP (UNKNOWN,	UNK),
	FTYPEMAP (PLT,		PLT),
	FTYPEMAP (COM,		COM),
	FTYPEMAP (NAV,		NAV),
	FTYPEMAP (FHQ,		FHQ),
};

static GUI_FTYPE_MAP *ftypemap_cache[SPWOOB_FTYPE_CNT] = { 0 };

QColor *
RES_GUI_color (SPWOOB_FTYPE id)
{
	GUI_FTYPE_MAP	*p;

	if (!(p = ftypemap_cache[id])) {
		for (int i=0; i<SPWOOB_FTYPE_CNT; i++)
			if (ftypemap[i].id == id) { p = &(ftypemap[i]); break; }
		ftypemap_cache[id] = p;
	}
	if (!p) return (RES_color (RID_GMOC_BG_DEFAULT));
	return (RES_color (p->rid));
}