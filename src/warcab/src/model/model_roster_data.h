/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_ROSTER_DATA_H
#define	MODEL_ROSTER_DATA_H	1

#include "../common.h"
#include "model.h"
#include "model_sort.h"

typedef struct s_MDLR_DATA {
	int			idx;
	SPWAW_UHTE		*uhte;
	bool			decomm;
	SPWAW_DOSSIER_UIR	*uir;
	SPWDLT			*dlt;
	bool			*dltsort;
	bool			*revsort;
} MDLR_DATA;

typedef struct s_MDLR_SMAP {
	MDLR_DATA		*data;
} MDLR_SMAP;

typedef enum e_MDLR_COLUMN {
	MDLR_COLUMN_CDSTATUS = 0,
	MDLR_COLUMN_UID,
	MDLR_COLUMN_UNIT,
	MDLR_COLUMN_RNK,
	MDLR_COLUMN_LDR,
	MDLR_COLUMN_STATUS,
	MDLR_COLUMN_KILL,
	MDLR_COLUMN_EXP,
	MDLR_COLUMN_MOR,
	MDLR_COLUMN_SUP,
	MDLR_COLUMN_RAL,
	MDLR_COLUMN_INF,
	MDLR_COLUMN_ARM,
	MDLR_COLUMN_ART,
	MDLR_COLUMN_MEN,
	MDLR_COLUMN_RDY,
	MDLR_COLUMN_KIA,
	MDLR_COLUMN_DMG,
	MDLR_COLUMN_SEEN,
	MDLR_COLUMN_ABAND,
	MDLR_COLUMN_LOADED,
	MDLR_COLUMN_TYPE,
	MDLR_COLUMN_CLASS,
	MDLR_COLUMN_COST,
	MDLR_COLUMN_SPEED,
	MDLR_COLUMN_LAST = MDLR_COLUMN_SPEED
} MDLR_COLUMN;

#define	MDLR_COLUMN_CNT	(MDLR_COLUMN_LAST + 1)

#define	MDLR_ROLE_SORT	Qt::UserRole

extern void	MDLR_data_sort	(int col, MDLR_DATA *p, MDL_CMPDATA &v);


#endif	/* MODEL_ROSTER_DATA_H */
