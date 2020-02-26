/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_UNITLIST_DATA_H
#define	MODEL_UNITLIST_DATA_H	1

#include "../common.h"
#include "model.h"

typedef struct s_MDLU_DATA {
	int			idx;
	SPWAW_UHTE		*uhte;
	bool			decomm;
	SPWAW_DOSSIER_UIR	*uir;
} MDLU_DATA;

typedef enum e_MDLU_COLUMN {
	MDLU_COLUMN_ID = 0,
	MDLU_COLUMN_LAST = MDLU_COLUMN_ID
} MDLU_COLUMN;

#define	MDLU_COLUMN_CNT	(MDLU_COLUMN_LAST + 1)

#endif	/* MODEL_UNITLIST_DATA_H */
