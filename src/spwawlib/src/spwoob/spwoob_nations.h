/** \file
* The SPWaW Library - OOB handling - nations handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"

#ifndef	SPWOOB_NATIONS_H
#define	SPWOOB_NATIONS_H	1

typedef struct s_OOBSTRINGS {
	const char	*people;
	const char	*nation;
	const char	*prefix;
	const char	*flagid;
} OOBSTRINGS;

typedef struct s_HISTEL {
	int		from_year;
	int		from_month;
	OOBSTRINGS	strings;
} HISTEL;

typedef struct s_HISTORY {
	int		cnt;
	const HISTEL	*list;
} HISTORY;

typedef struct s_IDMAP {
	BYTE		id;
	OOBSTRINGS	strings;
	int		start_year;
	int		start_month;
	int		end_year;
	int		end_month;
	const HISTORY	*history;
} IDMAP;

/*! Simple macro to calculate the length of a (statically initialized) array */
#define	ARRAYCOUNT(arr_) (sizeof(arr_)/sizeof(arr_[0]))

/*! Simple macro to quickly generate a conts static HISTORY */
#define	mkHISTORY(name_)	static const HISTORY name_ = { ARRAYCOUNT(name_ ## _list), name_ ## _list };

#endif	/* SPWOOB_NATIONS_H */
