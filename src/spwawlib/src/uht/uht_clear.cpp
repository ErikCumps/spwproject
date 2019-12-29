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

SPWAW_ERROR
UHT_clean (SPWAW_UHT *uht)
{
	CNULLARG (uht);

	for (unsigned int i=0; i<uht->blist.cnt; i++) {
		if (!uht->blist.info[i]->list) continue;

		memset (uht->blist.info[i]->list, 0,
			uht->blist.info[i]->cnt * sizeof (*(uht->blist.info[i]->list)));
	}

	for (unsigned int i=0; i<uht->cnt; i++) {
		if (!uht->list[i]) continue;
				
		if (uht->list[i]->lname) STRTAB_del ((STRTAB*)uht->dossier->stab, uht->list[i]->lname);
		if (uht->list[i]->UID) STRTAB_del ((STRTAB*)uht->dossier->stab, uht->list[i]->UID);
		if (uht->list[i]->uname) STRTAB_del ((STRTAB*)uht->dossier->stab, uht->list[i]->uname);
		safe_free (uht->list[i]);
	}

	for (unsigned int i=0; i<uht->cnt; i++) {
		if (!uht->smap[i]) continue;

		uht->smap[i] = NULL;
	}

	uht->cnt = 0;
	
	return (SPWERR_OK);
}

SPWAW_ERROR
UHT_clear (SPWAW_UHT *uht)
{
	SPWAW_ERROR	rc;

	CNULLARG (uht);

	rc = UHT_clean (uht); ROE ("UHT_clean()");

	if (uht->blist.info) {
		for (unsigned int i=0; i<uht->blist.cnt; i++) {
			if (uht->blist.info[i]->list) safe_free (uht->blist.info[i]->list);
			safe_free (uht->blist.info[i]);
		}
		safe_free (uht->blist.info);
		uht->blist.cnt = uht->blist.len = 0;
	}
	if (uht->list) safe_free (uht->list);
	if (uht->smap) safe_free (uht->smap);
	uht->cnt = uht->len = 0;

	return (SPWERR_OK);
}
