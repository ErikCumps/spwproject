/** \file
 * The SPWaW war cabinet - utility functions - simple string hash class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UTIL_STRHASH_H
#define	UTIL_STRHASH_H	1

#include <ad_hash.h>

/*! Simple string hash class */
class UtilStrhash
{
public:
	UtilStrhash	(void);
	~UtilStrhash	(void);

private:
	struct s_data {
		ADhash_t	hash;
	} d;
};

#endif	/* UTIL_STRHASH_H */
