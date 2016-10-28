/** \file
 * The SPWaW war cabinet - utility functions - simple string hash class.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "util_strbuf.h"

#define	HASHBUCKETS	211

// djb2 string hash function
static inline unsigned long
keyhash (char *str)
{
	unsigned long	h = 5381;
	int		c;

	while (c = *str++) h = ((h << 5) + h) + c; /* h * 33 + c */

	return (h);
}

static inline unsigned long
idforkey (void *key)
{
	unsigned long	id;

	if (key) {
		id = keyhash ((char *)key);
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
	return (!strcmp ((char *)key1, (char *)key2));
}

UtilStrhash::UtilStrhash (void)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.hash = ADhash_setup (HASHBUCKETS, hash_func, hash_match, NULL);
}

UtilStrhash::~UtilStrhash (void)
{
	if (d.hash) ADhash_destroy (d.hash);
}

