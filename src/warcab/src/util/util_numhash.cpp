/** \file
 * The SPWaW war cabinet - utility functions - simple number hash class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_strbuf.h"

#define	HASHBUCKETS	211

// number hash function
static inline unsigned long
keyhash (unsigned int num)
{
	return ((unsigned long)num);
}

static inline unsigned long
idforkey (void *key)
{
	unsigned long	id;

	if (key) {
		id = keyhash ((unsigned int)key);
	} else {
		id = 0;
	}

	return (id);
}

static unsigned int
hash_func (void *key)
{
	return (idforkey (key) % HASHBUCKETS);
}

static int
hash_match (void *key1, void *key2)
{
	return ((unsigned int)key1 == (unsigned int)key2);
}

UtilNumhash::UtilNumhash (void)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.hash = ADhash_setup (HASHBUCKETS, hash_func, hash_match, NULL);
}

UtilNumhash::~UtilNumhash (void)
{
	if (d.hash) ADhash_destroy (d.hash);
}

