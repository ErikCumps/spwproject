/** \file
 * The SPWaW war cabinet - utility functions - simple number hash class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_NUMHASH_H
#define	UTIL_NUMHASH_H	1

#include <ad_hash.h>

/*! Simple number hash class */
class UtilNumhash
{
public:
	UtilNumhash	(void);
	~UtilNumhash	(void);

private:
	struct s_data {
		ADhash_t	hash;
	} d;
};

#endif	/* UTIL_NUMHASH_H */
