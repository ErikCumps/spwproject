/** \file
 * The SPWaW war cabinet - data model handling - min-max-average-spread data.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PLOT_MODEL_MMAS_DATA_H
#define	PLOT_MODEL_MMAS_DATA_H	1

#include "../common.h"
#include "model.h"

typedef struct s_MDLMMAS_DATA {
	int			idx;
	union u_date {
		SPWAW_BATTLE_DATE	bdate;
		SPWAW_TURN_DATE		tdate;
	} date;
	SPWAW_SNAP_OOB_FORCE	*force;
	SPWDLT			*dlt;
} MDLMMAS_DATA;

typedef enum e_MDLMMAS_COLUMN {
	MDLMMAS_COLUMN_DATE = 0,
	MDLMMAS_COLUMN_MAX,
	MDLMMAS_COLUMN_MAXID,
	MDLMMAS_COLUMN_MIN,
	MDLMMAS_COLUMN_MINID,
	MDLMMAS_COLUMN_AVG,
	MDLMMAS_COLUMN_SPR,
	MDLMMAS_COLUMN_COUNT,
	MDLMMAS_COLUMN_LAST = MDLMMAS_COLUMN_COUNT,
	MDLMMAS_COLUMN_NONE
} MDLMMAS_COLUMN;

#define	MDLMMAS_COLUMN_CNT	(MDLMMAS_COLUMN_LAST + 1)

typedef struct s_MDLMMAS_COLUMN_DEF {
	MDLMMAS_COLUMN	id;
	const char	*name;
	SPWDLT_TYPE	dtype;
	unsigned int	base;
	unsigned int	item;
	unsigned int	member;
} MDLMMAS_COLUMN_DEF;

extern MDLMMAS_COLUMN_DEF *MDLMMAS_coldef (int col);

#endif	/* PLOT_MODEL_MMAS_DATA_H */
