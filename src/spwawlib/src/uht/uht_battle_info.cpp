/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "uht/uht.h"
#include "common/internal.h"

#define	UHT_LISTCNT_GROW	16

static SPWAW_ERROR
uht_new_binfo (SPWAW_UHT *uht, SPWAW_BATTLE *b, SPWAW_UHT_BINFO **dst)
{
	SPWAW_ERROR	rc;
	SPWAW_UHT_BINFO	*info = NULL;
	unsigned int	idx;

	CNULLARG (uht); CNULLARG (b); CNULLARG (dst);
	*dst = NULL;

	info = safe_malloc (SPWAW_UHT_BINFO); COOMGOTO (info, "SPWAW_UHT_BINFO", handle_error);
	info->bdate = b->bdate;
	info->cnt   = b->racnt;
	info->list  = safe_nmalloc (SPWAW_UHTE *, b->racnt); COOMGOTO (info->list, "SPWAW_UHT_BINFO list", handle_error);

	if (uht->blist.cnt >= uht->blist.len) {
		unsigned int nlen = uht->blist.len + UHT_LISTCNT_GROW;

		SPWAW_UHT_BINFO **nlist = (SPWAW_UHT_BINFO **)safe_realloc (uht->blist.info, nlen * sizeof (SPWAW_UHT_BINFO *));
		COOMGOTO (nlist, "SPWAW_UHT_BLIST", handle_error);

		uht->blist.info = nlist;
		uht->blist.len  = nlen;
	}

	idx = uht->blist.cnt++;
	b->uhtinfo = *dst = uht->blist.info[idx] = info;
	
	return (SPWERR_OK);

handle_error:
	if (info) {
		if (info->list) safe_free (info->list);
		safe_free (info);
	}
	return (rc);
}

SPWAW_ERROR
UHT_battle_info (SPWAW_UHT *uht, SPWAW_BATTLE *b, SPWAW_UHT_BINFO **info)
{
	CNULLARG (uht); CNULLARG (b); CNULLARG (info);
	*info = NULL;

	for (unsigned int i=0; i<uht->blist.cnt; i++) {
		if (SPWAW_bdate_cmp (&(uht->blist.info[i]->bdate), &(b->bdate)) == 0) {
			b->uhtinfo = *info = uht->blist.info[i];
			return (SPWERR_OK);
		}
	}

	return (uht_new_binfo (uht, b, info));
}