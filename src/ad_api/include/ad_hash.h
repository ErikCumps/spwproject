/** \file
 * The Aggregate Data API: the hash interface.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	AD_HASH_H
#define	AD_HASH_H	1

#include "ad_common.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *		ADhash_t;
typedef unsigned int	(*ADhash_hash_t)	(void *key);
typedef int 		(*ADhash_match_t)	(void *key1, void *key2);

extern ADhash_t		ADhash_setup		(unsigned int buckets, ADhash_hash_t hf, ADhash_match_t mf, AD_destructor_t df);
extern void		ADhash_clear		(ADhash_t hash);
extern void		ADhash_destroy		(ADhash_t hash);
extern AD_size_t	ADhash_size		(ADhash_t hash);

extern void *		ADhash_add		(ADhash_t hash, void *key, void *data);
extern void *		ADhash_del		(ADhash_t hash, void *key);
extern void *		ADhash_get		(ADhash_t hash, void *key);
extern AD_size_t	ADhash_keys		(ADhash_t hash, void **dst, AD_size_t len);

typedef struct {
	unsigned int	total;
	unsigned int	used;
	unsigned int	min_fill;
	unsigned int	min_count;
	unsigned int	max_fill;
	unsigned int	max_count;
	double		avg_fill;
} ADhash_stats_t;

extern void		ADhash_stats		(ADhash_t hash, ADhash_stats_t *stats);

extern void		debug_ADhash_report	(FILE *rf, ADhash_t hash, char *msg);

#ifdef __cplusplus
}
#endif

#endif	/* !AD_HASH_H */

