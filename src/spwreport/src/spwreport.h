/** \file
 * The Steel Panthers World at War report tool.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL V2
 */

#ifndef	INTERNAL_REPORT_H
#define	INTERNAL_REPORT_H	1

#include "stdafx.h"

#define	TURNTIME	SPWAW_MINSPERTURN
#define	HEXSIZE		SPWAW_HEXSIZE

#define	HEX2M(h_)	(h_ * HEXSIZE)
#define	HEX2K(h_)	((double)(h_ * HEXSIZE) / 1000.0)
#define	HEX2KPH(h_)	(HEX2K(h_) * 60 / TURNTIME)


extern void	raw_report		(SPWAW_SNAPSHOT *ptr, FILE *rf);
extern void	rawlink_report		(SPWAW_SNAPSHOT *ptr, FILE *rf);
extern void	complete_report		(SPWAW_SNAPSHOT *ptr, FILE *rf, bool core);
extern void	short_report		(SPWAW_SNAPSHOT *ptr, FILE *rf, bool core);
extern void	narrative_report	(SPWAW_SNAPSHOT *ptr, FILE *rf, bool core);
extern void	table_report		(SPWAW_SNAPSHOT *ptr, FILE *rf, bool core);
extern void	map_dump		(SPWAW_SNAPSHOT *ptr, FILE *rf);

#endif	/* INTERNAL_REPORT_H */
