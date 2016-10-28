/** \file
 * The SPWaW war cabinet - data model handling - order of battle data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_OOB_SORT_H
#define	MODEL_OOB_SORT_H	1

#include "model.h"
#include "model/model_sort.h"

extern int 	MDLO_cmp_fid	(const void *a, const void *b);
extern int 	MDLO_cmp_type	(const void *a, const void *b);
extern int 	MDLO_cmp_ldr	(const void *a, const void *b);
extern int 	MDLO_cmp_hcmd	(const void *a, const void *b);
extern int 	MDLO_cmp_status	(const void *a, const void *b);
extern int 	MDLO_cmp_kills	(const void *a, const void *b);
extern int 	MDLO_cmp_rdy	(const void *a, const void *b);
extern int 	MDLO_cmp_cnt	(const void *a, const void *b);
extern int 	MDLO_cmp_exp	(const void *a, const void *b);
extern int 	MDLO_cmp_mor	(const void *a, const void *b);
extern int 	MDLO_cmp_sup	(const void *a, const void *b);
extern int 	MDLO_cmp_ral	(const void *a, const void *b);
extern int 	MDLO_cmp_inf	(const void *a, const void *b);
extern int 	MDLO_cmp_arm	(const void *a, const void *b);
extern int 	MDLO_cmp_art	(const void *a, const void *b);

#endif	/* MODEL_OOB_SORT_H */
