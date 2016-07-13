/** \file
 * Resource Archive - buildres tool - ID (value) handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <stdlib.h>
#include <string.h>

#include <system/independence.h>
#include <ad_hash.h>
#include "idhash.h"

#define	BUCKETS		251
#define	IDFORKEY(key)	((unsigned long)key)

static unsigned int
hash_func (void *key)
{
        return (IDFORKEY (key) % BUCKETS);
}

static int
hash_match (void *key1, void *key2)
{
        return (IDFORKEY (key1) == IDFORKEY (key2));
}

static ADhash_t hash;

void
ih_init (void)
{
	hash = ADhash_setup (BUCKETS, hash_func, hash_match, NULL);
	if (hash == NULL) abort();
}

void
ih_shutdown (void)
{
	ADhash_destroy (hash);
}

void *
ih_find (unsigned long id)
{
	return (ADhash_get (hash, (void *)id));
}

void
ih_register (unsigned long id)
{
	if (ADhash_add (hash, (void *)id, (void *)id) == NULL) abort();
}

