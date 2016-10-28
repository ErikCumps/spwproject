/** \file
 * The SPWaW war cabinet - data model handling - unit data.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	MODEL_ROSTER_SORT_H
#define	MODEL_ROSTER_SORT_H	1

#include "model.h"
#include "model/model_sort.h"

extern int 	MDLR_cmp_uid	(const void *a, const void *b);
extern int 	MDLR_cmp_unit	(const void *a, const void *b);
extern int 	MDLR_cmp_rnk	(const void *a, const void *b);
extern int 	MDLR_cmp_ldr	(const void *a, const void *b);
extern int 	MDLR_cmp_type	(const void *a, const void *b);
extern int 	MDLR_cmp_class	(const void *a, const void *b);
extern int 	MDLR_cmp_kill	(const void *a, const void *b);
extern int 	MDLR_cmp_status	(const void *a, const void *b);
extern int 	MDLR_cmp_seen	(const void *a, const void *b);
extern int 	MDLR_cmp_exp	(const void *a, const void *b);
extern int 	MDLR_cmp_mor	(const void *a, const void *b);
extern int 	MDLR_cmp_sup	(const void *a, const void *b);
extern int 	MDLR_cmp_ral	(const void *a, const void *b);
extern int 	MDLR_cmp_inf	(const void *a, const void *b);
extern int 	MDLR_cmp_art	(const void *a, const void *b);
extern int 	MDLR_cmp_arm	(const void *a, const void *b);
extern int 	MDLR_cmp_men	(const void *a, const void *b);
extern int 	MDLR_cmp_kia	(const void *a, const void *b);
extern int 	MDLR_cmp_dmg	(const void *a, const void *b);
extern int 	MDLR_cmp_rdy	(const void *a, const void *b);
extern int 	MDLR_cmp_aband	(const void *a, const void *b);
extern int 	MDLR_cmp_loaded	(const void *a, const void *b);
extern int 	MDLR_cmp_cost	(const void *a, const void *b);
extern int 	MDLR_cmp_speed	(const void *a, const void *b);

#endif	/* MODEL_ROSTER_SORT_H */
