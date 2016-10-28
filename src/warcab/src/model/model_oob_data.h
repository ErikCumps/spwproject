/** \file
 * The SPWaW war cabinet - data model handling - order of battle data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_OOB_DATA_H
#define	MODEL_OOB_DATA_H	1

#include "../common.h"
#include "model.h"
#include "model_sort.h"

typedef enum e_MDLO_DATA_TYPE {
	MDLO_DATA_FORM = 0,
	MDLO_DATA_UNIT
} MDLO_DATA_TYPE;

typedef struct s_MDLO_DATA {
	MDLO_DATA_TYPE			type;
	struct s_MDLO_DATA		*parent;
	int				idx;
	union u_data {
		SPWAW_DOSSIER_FIR	*f;
		SPWAW_DOSSIER_UIR	*u;
	}				data;
	SPWDLT				*dlt;
	bool				*dltsort;
	bool				*revsort;
	int				ccnt;
	struct s_MDLO_DATA		*clst;
	SPWDLT				*cdlt;
} MDLO_DATA;

typedef struct s_v {
	MDLO_DATA			*data;
} MDLO_SMAP;

typedef struct s_MDLO_EXPLIST {
	SPWAW_DOSSIER_BIR	*ref;
	int			cnt;
	bool			*lst;
} MDLO_EXPLIST;

typedef enum e_MDLO_COLUMN {
	MDLO_COLUMN_FID = 0,
	MDLO_COLUMN_TYPE,
	MDLO_COLUMN_LDR,
	MDLO_COLUMN_HCMD,
	MDLO_COLUMN_STATUS,
	MDLO_COLUMN_KILLS,
	MDLO_COLUMN_RDY,
	MDLO_COLUMN_COUNT,
	MDLO_COLUMN_EXP,
	MDLO_COLUMN_MOR,
	MDLO_COLUMN_SUP,
	MDLO_COLUMN_RAL,
	MDLO_COLUMN_INF,
	MDLO_COLUMN_ARM,
	MDLO_COLUMN_ART,
	MDLO_COLUMN_LAST = MDLO_COLUMN_ART
} MDLO_COLUMN;

#define	MDLO_COLUMN_CNT	(MDLO_COLUMN_LAST + 1)

#define	MDLO_ROLE_SORT	Qt::UserRole

extern void	MDLO_data_sort	(int col, MDLO_DATA *p, MDL_CMPDATA &v);

#endif	/* MODEL_OOB_DATA_H */
