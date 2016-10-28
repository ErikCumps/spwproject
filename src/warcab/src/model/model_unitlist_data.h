/** \file
 * The SPWaW war cabinet - data model handling - unit list.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_UNITLIST_DATA_H
#define	MODEL_UNITLIST_DATA_H	1

#include "../common.h"
#include "model.h"

typedef enum e_MDLUL_COLUMN {
	MDLUL_COLUMN_ID = 0,
	MDLUL_COLUMN_LAST = MDLUL_COLUMN_ID
} MDLUL_COLUMN;

#define	MDLUL_COLUMN_CNT	(MDLUL_COLUMN_LAST + 1)

#endif	/* MODEL_UNITLIST_DATA_H */
