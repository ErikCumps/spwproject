/** \file
 * The SPWaW Library - SPWaW OOB list handling.
 *
 * Copyright (C) 2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "spwoob_list.h"
#include "common/internal.h"

#define	LISTINC	8
#define	IDXNONE	((unsigned long)-1)

SPWAW_ERROR
SPWOOB_LIST_new (SPWAW_SPWOOB_LIST **list)
{
	SPWAW_SPWOOB_LIST	*p = NULL;

	CNULLARG (list);
	*list = NULL;

	p = safe_malloc (SPWAW_SPWOOB_LIST); COOM (p, "SPWAW_SPWOOB_LIST");

	*list = p;

	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_free (SPWAW_SPWOOB_LIST **list)
{
	SPWAW_SPWOOB_LIST	*l;

	CNULLARG (list);
	l = *list; *list = NULL;

	if (l) {
		if (l->list) {
			for (unsigned int i=0; i<l->cnt; i++) {
				if (l->list[i]) {
					if (l->list[i]->data) SPWOOB_free (&(l->list[i]->data));
					free (l->list[i]);
				}
			}
			free (l->list);
		}
		free (l);
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_add (SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob, unsigned long *idx)
{
	SPWAW_ERROR		rc;
	unsigned long		lidx;
	SPWAW_SPWOOB_LIST_NODE	*p;
	SPWAW_SPWOOB_LIST_NODE	**l;

	CNULLARG (list); CNULLARG (spwoob);

	rc = SPWOOB_LIST_match (list, spwoob, &lidx);

	if (HASERROR) {
		p = safe_malloc (SPWAW_SPWOOB_LIST_NODE); COOMGOTO (p, "SPWAW_SPWOOB_LIST_NODE node", handle_error);
		rc = SPWOOB_new (&(p->data)); ERRORGOTO("SPWOOB_new() failed", handle_error);
		rc = SPWOOB_copy (p->data, spwoob); ERRORGOTO("SPWOOB_copy() failed", handle_error);
		p->refcnt = 1;

		if (list->cnt >= list->len) {
			list->len += LISTINC;
			l = safe_nmalloc (SPWAW_SPWOOB_LIST_NODE *, list->len); COOMGOTO (l, "SPWAW_SPWOOB_LIST_NODE list", handle_error);

			if (list->list) {
				memcpy (l, list->list, list->cnt * sizeof (SPWAW_SPWOOB_LIST_NODE *));
				free (list->list);
			}
			list->list = l;
		}
		lidx = list->cnt++;
		list->list[lidx] = p;
	} else {
		list->list[lidx]->refcnt++;
	}

	if (idx) *idx = lidx;

	return (SPWERR_OK);

handle_error:
	if (p->data) SPWOOB_free (&(p->data));
	if (p) free (p);
	return (rc);
}

SPWAW_ERROR
SPWOOB_LIST_del (SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob)
{
	bool		found = false;
	unsigned int	i;

	CNULLARG (list); CNULLARG (spwoob);

	for (i=0; i<list->cnt; i++) {
		if (list->list[i]->data == spwoob) {
			found = true;
			list->list[i]->refcnt--;
			if (list->list[i]->refcnt == 0) SPWOOB_free (&(list->list[i]->data));
			break;
		}
	}
	if (!found) RWE (SPWERR_FAILED, "entry not found");

	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_match (SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob, unsigned long *idx)
{
	bool		found = false;
	unsigned int	i;

	CNULLARG (list); CNULLARG (spwoob); CNULLARG(idx);
	*idx = IDXNONE;

	// Pass #1 - simple match
	found = false;
	for (i=0; i<list->cnt; i++) {
		if (list->list[i]->data == spwoob) {
			found = true;
			break;
		}
	}
	if (found) {
		*idx = i;
		return (SPWERR_OK);
	}

	// Pass #2 - deep match
	found = false;
	for (i=0; i<list->cnt; i++) {
		if (SPWOOB_compare (spwoob, list->list[i]->data) == SPWERR_OK) {
			found = true;
			break;
		}
	}
	if (found) {
		*idx = i;
		return (SPWERR_OK);
	}

	return (SPWERR_FAILED);
}

SPWAW_ERROR
SPWOOB_LIST_clear (SPWAW_SPWOOB_LIST *list)
{
	unsigned int	i;

	CNULLARG (list);

	for (i=0; i<list->cnt; i++) {
		if (list->list[i]->data) SPWOOB_free (&(list->list[i]->data));
		list->list[i]->refcnt = 0;
	}
	list->cnt = 0;

	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_compress (SPWAW_SPWOOB_LIST *list)
{
	unsigned int	i, j;

	CSPWINIT;
	CNULLARG (list);

	// Detect first non-empty node
	for (i=0; i<list->cnt; i++) {
		if (list->list[i]->data) break;
	}

	// Early exit if all nodes are empty
	if (i >= list->cnt) return (SPWERR_OK);

	// Remove leading empty nodes first, if any
	if (i > 0) {
		for (j=i; i<list->cnt; i++) {
			memcpy (list->list[i-j], list->list[i], sizeof(list->list[i]));
			memset (list->list[i], 0, sizeof(list->list[i]));
		}
	}

	// Remove all other empty nodes, if any
	for (i=j=0; i<list->cnt; i++) {
		if (list->list[i]->refcnt == 0) continue;
		if (i != j) {
			memcpy (list->list[j], list->list[i], sizeof(list->list[i]));
			memset (list->list[i], 0, sizeof(list->list[i]));
		}
		j++;
	}
	list->cnt = j;

	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_idx2spwoob (SPWAW_SPWOOB_LIST *list, unsigned long idx, SPWOOB **spwoob)
{
	CNULLARG (list); CNULLARG (spwoob);
	*spwoob = NULL;

	if (idx >= list->cnt) RWE (SPWERR_FAILED, "invalid idx");

	*spwoob = list->list[idx]->data;
	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_spwoob2idx (SPWAW_SPWOOB_LIST *list, SPWOOB *spwoob, unsigned long *idx)
{
	bool		found = false;
	unsigned int	i;

	CNULLARG (list); CNULLARG (idx);
	*idx = IDXNONE;

	for (i=0; i<list->cnt; i++) {
		if (list->list[i]->data == spwoob) {
			found = true;
			break;
		}
	}
	if (!found) RWE (SPWERR_FAILED, "entry not found");

	*idx = i;
	return (SPWERR_OK);
}

SPWAW_ERROR
SPWOOB_LIST_debug_log (SPWAW_SPWOOB_LIST *list)
{
	unsigned int	i;

	CNULLARG (list);

	log ("SPWAW_SPWOOB_LIST: cnt=%lu, len=%lu\n", list->cnt, list->len);
	for (i=0; i<list->cnt; i++) {
		log ("#%01.1lu: 0x%8.8x (refcnt %lu)\n", i, list->list[i]->data, list->list[i]->refcnt);
	}
	return (SPWERR_OK);
}
