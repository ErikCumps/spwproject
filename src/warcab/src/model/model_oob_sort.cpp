/** \file
 * The SPWaW war cabinet - data model handling - order of battle data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "model_sort.h"
#include "model_oob_data.h"

static MDL_CMPDEF defs[] = {
	{ MDLO_COLUMN_FID,	1,	{ { MDL_CMPINT, MDLO_COLUMN_FID, 0 }	} },
	{ MDLO_COLUMN_TYPE,	1,	{ { MDL_CMPINT, MDLO_COLUMN_TYPE, 0 }	} },
	{ MDLO_COLUMN_LDR,	1,	{ { MDL_CMPINT, MDLO_COLUMN_LDR, 0 }	} },
	{ MDLO_COLUMN_HCMD,	1,	{ { MDL_CMPSTR, MDLO_COLUMN_HCMD, 0 }	} },
	{ MDLO_COLUMN_STATUS,	1,	{ { MDL_CMPINT, MDLO_COLUMN_STATUS, 0 }	} },
	{ MDLO_COLUMN_KILLS,	1,	{ { MDL_CMPINT, MDLO_COLUMN_KILLS, 1 }	} },
	{ MDLO_COLUMN_RDY,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_RDY, 1  }	} },
	{ MDLO_COLUMN_COUNT,	1,	{ { MDL_CMPINT, MDLO_COLUMN_COUNT, 0 }	} },
	{ MDLO_COLUMN_EXP,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_EXP, 1 }	} },
	{ MDLO_COLUMN_MOR,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_MOR, 1 }	} },
	{ MDLO_COLUMN_SUP,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_SUP, 1 }	} },
	{ MDLO_COLUMN_RAL,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_RAL, 1 }	} },
	{ MDLO_COLUMN_INF,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_INF, 1 }	} },
	{ MDLO_COLUMN_ARM,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_ARM, 1 }	} },
	{ MDLO_COLUMN_ART,	1,	{ { MDL_CMPDBL, MDLO_COLUMN_ART, 1 }	} },
	{ -1, 0 },
};

static MDL_CMPDEF *lookup_def_cache[MDLO_COLUMN_CNT] = { 0 };

static inline MDL_CMPDEF *
lookup_def (MDLO_COLUMN col)
{
	MDL_CMPDEF	*p = NULL;
	int		i;

	if (p = lookup_def_cache[col]) return (p);

	i = 0;
	while (defs[i].ID != -1) {
		if (defs[i].ID == col) { p =&(defs[i]); break; }
		i++;
	}
	return (lookup_def_cache[col] = p);
}

int MDLO_cmp_fid	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_FID))); }
int MDLO_cmp_type	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_TYPE))); }
int MDLO_cmp_ldr	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_LDR))); }
int MDLO_cmp_hcmd	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_HCMD))); }
int MDLO_cmp_status	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_STATUS))); }
int MDLO_cmp_kills	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_KILLS))); }
int MDLO_cmp_rdy	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_RDY))); }
int MDLO_cmp_cnt	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_COUNT))); }
int MDLO_cmp_exp	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_EXP))); }
int MDLO_cmp_mor	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_MOR))); }
int MDLO_cmp_sup	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_SUP))); }
int MDLO_cmp_ral	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_RAL))); }
int MDLO_cmp_inf	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_INF))); }
int MDLO_cmp_arm	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_ARM))); }
int MDLO_cmp_art	(const void *a, const void *b) { return (MDL_stdcmp_GMO (a, b, lookup_def (MDLO_COLUMN_ART))); }
