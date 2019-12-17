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

	if (uht->list) {
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
	}
	
	return (SPWERR_OK);
}

SPWAW_ERROR
UHT_clear (SPWAW_UHT *uht)
{
	SPWAW_ERROR	rc;

	CNULLARG (uht);

	rc = UHT_clean (uht); ROE ("UHT_clean()");

	if (uht->list) safe_free (uht->list);
	if (uht->smap) safe_free (uht->smap);
	uht->len = 0;

	return (SPWERR_OK);
}
