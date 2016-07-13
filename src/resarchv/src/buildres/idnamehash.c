/** \file
 * Resource Archive - buildres tool - ID (name) hash handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdlib.h>
#include <string.h>

#include <system/independence.h>
#include <ad_hash.h>
#include "idnamehash.h"

#define	BUCKETS		111
#define	NMFORKEY(key)	((char *)key)

static unsigned int
hash_func (void *key)
{
	char		*p = NMFORKEY (key);
	unsigned int	V = 0, len;

	len = strlen (p);
	while (*p != '\0') {
		V = (V << 1) | (V >> ((sizeof(V)*8) - 1));
		V += *p;
		p++;
	}
	V += len;

        return (V % BUCKETS);
}

static int
hash_match (void *key1, void *key2)
{
	return (!strcmp (NMFORKEY (key1), NMFORKEY (key2)));
}

static ADhash_t hash;

void
nh_init (void)
{
	hash = ADhash_setup (BUCKETS, hash_func, hash_match, NULL);
	if (hash == NULL) abort();
}

void
nh_shutdown (void)
{
	ADhash_destroy (hash);
}

void *
nh_find (char *name)
{
	return (ADhash_get (hash, (void *)name));
}

void
nh_register (char *name, unsigned long id)
{
	if (ADhash_add (hash, (void *)name, (void *)id) == NULL) abort();
}
