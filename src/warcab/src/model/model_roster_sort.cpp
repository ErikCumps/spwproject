/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_roster.h"
#include "model_sort.h"

static MDL_CMPDEF defs[] = {
	{ MDLR_COLUMN_UID,	1,	{ { MDL_CMPUID, MDLR_COLUMN_UID, 0 }	} },
	{ MDLR_COLUMN_UNIT,	1,	{ { MDL_CMPSTR, MDLR_COLUMN_UNIT, 0 }	} },
	{ MDLR_COLUMN_RNK,	1,	{ { MDL_CMPINT, MDLR_COLUMN_RNK, 1 }	} },
	{ MDLR_COLUMN_LDR,	1,	{ { MDL_CMPSTR, MDLR_COLUMN_LDR, 0 }	} },
	{ MDLR_COLUMN_TYPE,	1,	{ { MDL_CMPINT, MDLR_COLUMN_TYPE, 0 }	} },
	{ MDLR_COLUMN_CLASS,	2,	{ { MDL_CMPINT, MDLR_COLUMN_CLASS, 0 },	{ MDL_CMPINT, MDLR_COLUMN_TYPE, 0 }	} },
	{ MDLR_COLUMN_KILL,	1,	{ { MDL_CMPINT, MDLR_COLUMN_KILL, 1  }	} },
	{ MDLR_COLUMN_STATUS,	1,	{ { MDL_CMPINT, MDLR_COLUMN_STATUS, 0 }	} },
	{ MDLR_COLUMN_SEEN,	1,	{ { MDL_CMPINT, MDLR_COLUMN_SEEN, 1 }	} },
	{ MDLR_COLUMN_EXP,	1,	{ { MDL_CMPINT, MDLR_COLUMN_EXP, 1 }	} },
	{ MDLR_COLUMN_MOR,	1,	{ { MDL_CMPINT, MDLR_COLUMN_MOR, 1 }	} },
	{ MDLR_COLUMN_SUP,	1,	{ { MDL_CMPINT, MDLR_COLUMN_SUP, 1 }	} },
	{ MDLR_COLUMN_RAL,	1,	{ { MDL_CMPINT, MDLR_COLUMN_RAL, 1 }	} },
	{ MDLR_COLUMN_INF,	1,	{ { MDL_CMPINT, MDLR_COLUMN_INF, 1 }	} },
	{ MDLR_COLUMN_ARM,	1,	{ { MDL_CMPINT, MDLR_COLUMN_ARM, 1 }	} },
	{ MDLR_COLUMN_ART,	1,	{ { MDL_CMPINT, MDLR_COLUMN_ART, 1 }	} },
	{ MDLR_COLUMN_MEN,	1,	{ { MDL_CMPINT, MDLR_COLUMN_MEN, 1 }	} },
	{ MDLR_COLUMN_KIA,	1,	{ { MDL_CMPINT, MDLR_COLUMN_KIA, 1 }	} },
	{ MDLR_COLUMN_DMG,	1,	{ { MDL_CMPINT, MDLR_COLUMN_DMG, 1 }	} },
	{ MDLR_COLUMN_RDY,	1,	{ { MDL_CMPDBL, MDLR_COLUMN_RDY, 1 }	} },
	{ MDLR_COLUMN_ABAND,	1,	{ { MDL_CMPINT, MDLR_COLUMN_ABAND, 1 }	} },
	{ MDLR_COLUMN_LOADED,	1,	{ { MDL_CMPINT, MDLR_COLUMN_LOADED, 1 }	} },
	{ MDLR_COLUMN_COST,	1,	{ { MDL_CMPINT, MDLR_COLUMN_COST, 1 }	} },
	{ MDLR_COLUMN_SPEED,	1,	{ { MDL_CMPINT, MDLR_COLUMN_SPEED, 1 }	} },
	{ -1, 0 },
};

static MDL_CMPDEF *lookup_def_cache[MDLR_COLUMN_CNT] = { 0 };

static inline MDL_CMPDEF *
lookup_def (MDLR_COLUMN col)
{
	MDL_CMPDEF	*p;
	int		i;

	if (p = lookup_def_cache[col]) return (p);

	i = 0;
	while (defs[i].ID != -1) {
		if (defs[i].ID == col) { p =&(defs[i]); break; }
		i++;
	}
	return (lookup_def_cache[col] = p);
}

int MDLR_cmp_uid	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_UID))); }
int MDLR_cmp_unit	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_UNIT))); }
int MDLR_cmp_rnk	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_RNK))); }
int MDLR_cmp_ldr	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_LDR))); }
int MDLR_cmp_type	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_TYPE))); }
int MDLR_cmp_class	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_CLASS))); }
int MDLR_cmp_kill	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_KILL))); }
int MDLR_cmp_status	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_STATUS))); }
int MDLR_cmp_seen	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_SEEN))); }
int MDLR_cmp_exp	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_EXP))); }
int MDLR_cmp_mor	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_MOR))); }
int MDLR_cmp_sup	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_SUP))); }
int MDLR_cmp_ral	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_RAL))); }
int MDLR_cmp_inf	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_INF))); }
int MDLR_cmp_arm	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_ARM))); }
int MDLR_cmp_art	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_ART))); }
int MDLR_cmp_men	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_MEN))); }
int MDLR_cmp_kia	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_KIA))); }
int MDLR_cmp_dmg	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_DMG))); }
int MDLR_cmp_rdy	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_RDY))); }
int MDLR_cmp_aband	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_ABAND))); }
int MDLR_cmp_loaded	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_LOADED))); }
int MDLR_cmp_cost	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_COST))); }
int MDLR_cmp_speed	(const void *a, const void *b) { return (MDL_stdcmp_GMR (a, b, lookup_def (MDLR_COLUMN_SPEED))); }
