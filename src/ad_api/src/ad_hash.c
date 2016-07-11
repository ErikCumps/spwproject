/** \file
 * The Aggregate Data API: the hash implementation.
 *
 * Copyright (C) 2008-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include <malloc.h>
#include <stdio.h>

/* \TODO allow concurrent access */

#include <ad_hash.h>
#include "ad_magic.h"
#include <debug_memtrace.h>

struct hash_element_t;

struct hash_t {
	unsigned int		magic;		/* Used to check validity of hash structure pointer */
	unsigned int		buckets;	/* Number of buckets in hash */
	ADhash_hash_t		keygen;		/* Hash function */
	ADhash_match_t		match;		/* Key match function */
	AD_destructor_t		destruct;	/* Optional payload destructor */
	struct hash_element_t	**hash;		/* Pointer to hash bucket array */
	signed long		size;		/* Current number of elements in hash */
};

struct hash_element_t {
	void			*key;		/* Hash element: key */
	void			*data;		/* Hash element: payload */
	struct hash_element_t	*next;		/* Pointer to next hash element in bucket */
	unsigned int		bucket;		/* Index of hash bucket for quick lookup */
};

ADhash_t
ADhash_setup (unsigned int buckets, ADhash_hash_t hf, ADhash_match_t mf, AD_destructor_t df)
{
	struct hash_t	*hash = NULL;
	unsigned int	i;

	/* Check arguments */
	if ((buckets == 0) || (hf == NULL) || (mf == NULL)) return NULL;

	/* Create new hash structure */
	NEW_ROF (hash, NULL);
	hash->magic    = ADMAGIC_HASH;
	hash->buckets  = buckets;
	hash->keygen   = hf;
	hash->match    = mf;
	hash->destruct = df;
	hash->hash     = NULL;
	hash->size     = 0;

	/* Create hash buckets */
	if ((hash->hash = malloc (buckets * sizeof (struct hash_element_t))) == NULL) {
		free (hash);
		return (NULL);
	}
	for (i = 0; i < hash->buckets; i++) hash->hash[i] = NULL;

	return ((ADhash_t)hash);
}

void
ADhash_clear (ADhash_t hash)
{
	struct hash_t		*h;
	unsigned int		i;
	struct hash_element_t	*p, *q;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return;

	/* Free hash elements */
	for (i = 0; i < h->buckets; i++) {
		p = h->hash[i]; h->hash[i] = NULL;
		while (p != NULL) {
			q = p; p = p->next;
			if (h->destruct != NULL) h->destruct (q->data);
			free (q);
		}
	}

	/* Reset element count */
	h->size = 0;
}

void
ADhash_destroy (ADhash_t hash)
{
	struct hash_t		*h;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return;

	/* Clear hash */
	ADhash_clear (hash);

	/* Invalidate hash structure */
	h->magic = 0;

	/* Free hash buckets and hash structure */
	free (h->hash);
	memset (h, 0, sizeof (struct hash_t)); free (h);
}

AD_size_t
ADhash_size (ADhash_t hash)
{
	struct hash_t		*h;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return (0);

	return (h->size);
}

static INLINE unsigned int
hash_index (struct hash_t *h, void *key)
{
	/* Make sure hash index does not exceed buckets! */
	return (h->keygen (key) % h->buckets);
}

static struct hash_element_t *
hash_lookup (struct hash_t *h, void *key)
{
	unsigned int		idx;
	struct hash_element_t	*p;

	/* Calculate hash index and check for empty bucket */
	idx = hash_index (h, key);
	if ((p = h->hash[idx]) == NULL) return (NULL);

	/* Check elements in bucket */
	while ((p != NULL) && (h->match (key, p->key) == 0)) p = p->next;
	return (p);
}

void *
ADhash_add (ADhash_t hash, void *key, void *data)
{
	struct hash_t		*h;
	struct hash_element_t	*p, *e;
	unsigned int		i;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return (NULL);

	/* First check for duplicate key */
	p = hash_lookup (h, key);
	if (p != NULL) return (NULL);

	/* Create new hash element */
	NEW_ROF (e, NULL);
	e->key   = key;
	e->data  = data;

	/* Calculate hash index and add element to bucket */
	i = hash_index (h, key); e->bucket = i;
	if ((p = h->hash[i]) == NULL) {
		h->hash[i] = e;
	} else {
		while (p->next != NULL) p = p->next;
		p->next = e;
	}
	h->size++;

	/* Return new element */
	return (data);
}

void *
ADhash_del (ADhash_t hash, void *key)
{
	struct hash_t		*h;
	struct hash_element_t	*p, *q;
	unsigned int		i;
	void			*data = NULL;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return (NULL);

	/* Look for hash element */
	p = hash_lookup (h, key);
	if (p == NULL) return (NULL);

	/* Remove element from bucket */
	i = p->bucket;
	q = h->hash[i];
	if (q == NULL) return (NULL);
	if (q == p) {
		h->hash[i] = p->next;
	} else {
		while ((q != NULL) && (q->next != p)) q = q->next;
		if (q == NULL) return (NULL);
		q->next = p->next;
	}
	h->size--;

	/* Delete element and handle payload */
	data = p->data;
	if (h->destruct != NULL) { h->destruct (data); data = NULL; }
	free (p);

	/* Return payload */
	return (data);
}

void *
ADhash_get (ADhash_t hash, void *key)
{
	struct hash_t		*h;
	struct hash_element_t	*p;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return (NULL);

	/* Look for hash element */
	p = hash_lookup (h, key);
	if (p == NULL) return (NULL);

	/* Return payload */
	return (p->data);
}

AD_size_t
ADhash_keys (ADhash_t hash, void **dst, AD_size_t len)
{
	struct hash_t		*h;
	unsigned long		left, idx, bucket;
	struct hash_element_t	*p;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return (0);

	/* Check destination buffer */
	if (!dst || !len) return (0);

	/* Copy keys to destination buffer */
	left = h->size; idx = bucket = 0; p = NULL;
	while ((idx < len) && left) {
		if (!p && ((p = h->hash[bucket++]) != NULL)) break;

		dst[idx++] = p; p = p->next; left--;
	}
	return (idx);
}

void
ADhash_stats (ADhash_t hash, ADhash_stats_t *stats)
{
	struct hash_t		*h;
	unsigned int		i, cnt, total;
	struct hash_element_t	*p;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return;

	/* Check arguments */
	if (stats == NULL) return;
	memset (stats, 0, sizeof (*stats));

	/* Calculate stats */
	stats->total = h->buckets;
	stats->min_fill = stats->max_fill = 0;
	total = 0;
	for (i=0; i<h->buckets; i++) {
		if ((p = h->hash[i]) == NULL) continue;
		stats->used++;
		cnt = 0; while (p != NULL) { cnt++; p = p->next; } total += cnt;
		if (!stats->min_fill || (cnt < stats->min_fill)) {
			stats->min_fill = cnt;
			stats->min_count = 0;
		}
		if (cnt == stats->min_fill) stats->min_count++;
		if (!stats->max_fill || (cnt > stats->max_fill)) {
			stats->max_fill = cnt;
			stats->max_count = 0;
		}
		if (cnt == stats->max_fill) stats->max_count++;
	}
	if (stats->used != 0)
		stats->avg_fill = (double)total / (double)stats->used;
	else
		stats->avg_fill = 0.0;
}

void
debug_ADhash_report (FILE *rf, ADhash_t hash, char *msg)
{
	struct hash_t		*h;
	unsigned int		i;
	struct hash_element_t	*p;

	/* Check hash structure validity */
	if ((h = check_magic (hash, ADMAGIC_HASH)) == NULL) return;

	fprintf (rf, "ADhash (%s) = 0x%8.8x\n", msg, h);

	fprintf (rf, "buckets = %lu\n", h->buckets);
	fprintf (rf, "size    = %lu\n", h->size);
	fprintf (rf, "\n");

	for (i=0; i<h->buckets; i++) {
		if (!h->hash[i]) continue;

		fprintf (rf, "bucket[%3lu]", i);
		p = h->hash[i];
		while (p) {
			fprintf (rf, " {\"%s\", %lu}", (char *)(p->key), (ULONG)p->data);
			p = p->next;
		}
		fprintf (rf, "\n");
	}
	fprintf (rf, "\n");
}
