/** \file
 * The SPWaW Library - unit history tracking API implementation.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_dossier.h>
#include <spwawlib_uht.h>
#include "dossier/dossier.h"

static inline SPWAW_UHTE *
uht_lookup (SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate)
{
	if (!base || !bdate) return (NULL);

	SPWAW_UHTE *u = base;

	while ((SPWAW_bdate_cmp(bdate, &(u->LBD)) > 0) && u->next) { u = u->next; }
	return (u);
}

SPWAWLIB_API bool
SPWAW_UHT_lookup (SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate, bool strict, SPWAW_UHTE **uhte, SPWAW_DOSSIER_UIR **sobuir, SPWAW_DOSSIER_UIR **eobuir)
{
	SPWAW_UHTE		*uptr = NULL;
	SPWAW_DOSSIER_UIR	*psobuir = NULL;
	SPWAW_DOSSIER_UIR	*peobuir = NULL;

	if (uhte) *uhte = NULL;
	if (sobuir) *sobuir = NULL;
	if (eobuir) *eobuir = NULL;

	uptr = uht_lookup (base, bdate);
	if (strict) {
		if ((SPWAW_bdate_cmp(bdate, &(uptr->FBD)) < 0) || (SPWAW_bdate_cmp(&(uptr->LBD), bdate) < 0))
			return (false);
	}

	if (sobuir || eobuir) {
		SPWAW_BATTLE *bptr = uptr->uht->dossier->bfirst;
		while ((SPWAW_bdate_cmp(&(bptr->bdate), &(uptr->FBD)) != 0) && bptr->next) bptr = bptr->next;
		if (SPWAW_bdate_cmp(&(bptr->bdate), &(uptr->FBD)) != 0) return (false);

		USHORT fui = uptr->FUI; SPWAW_BATTLE_DATE fbd = uptr->FBD;
		while ((SPWAW_bdate_cmp(&fbd, bdate) < 0) && bptr->next) {
			if (bptr->ra[fui].dst == SPWAW_BADIDX) break;
			fui = bptr->ra[fui].dst; fbd = bptr->next->bdate;
			bptr = bptr->next;
		}
		if (strict) {
			if (SPWAW_bdate_cmp(&(bptr->bdate), bdate) != 0) return (false);
		}

		psobuir = &(bptr->info_sob->pbir_core.uir[fui]);
		peobuir = &(bptr->info_eob->pbir_core.uir[fui]);
	}

	if (uhte) *uhte = uptr;
	if (sobuir) *sobuir = psobuir;
	if (eobuir) *eobuir = peobuir;

	return (true);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_lookup_UHTE (SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate, bool strict)
{
	SPWAW_UHTE *uhte = NULL;

	if (SPWAW_UHT_lookup (base, bdate, strict, &uhte, NULL, NULL)) {
		return (uhte);
	} else {
		return (NULL);
	}
}

SPWAWLIB_API SPWAW_DOSSIER_UIR *
SPWAW_UHT_lookup_SOBUIR (SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate, bool strict)
{
	SPWAW_DOSSIER_UIR	*uir  = NULL;

	if (SPWAW_UHT_lookup (base, bdate, strict, NULL, &uir, NULL)) {
		return (uir);
	} else {
		return (NULL);
	}
}

SPWAWLIB_API SPWAW_DOSSIER_UIR *
SPWAW_UHT_lookup_EOBUIR (SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate, bool strict)
{
	SPWAW_DOSSIER_UIR	*uir  = NULL;

	if (SPWAW_UHT_lookup (base, bdate, strict, NULL, NULL, &uir)) {
		return (uir);
	} else {
		return (NULL);
	}
}

SPWAWLIB_API bool
SPWAW_UHT_is_active (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	return ((SPWAW_bdate_cmp(bdate, &(uptr->FBD)) >= 0) && (SPWAW_bdate_cmp(bdate, &(uptr->LBD)) <= 0));
}

SPWAWLIB_API bool
SPWAW_UHT_is_commissioned (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	return ((SPWAW_bdate_cmp(bdate, &(uptr->FBD)) == 0) && !uptr->prev);
}

SPWAWLIB_API bool
SPWAW_UHT_is_commissioned (SPWAW_UHTE *uhte)
{
	SPWAW_UHTE *uptr = SPWAW_UHT_first (uhte);
	if (!uptr) return (false);

	return (SPWAW_bdate_cmp (&(uptr->uht->dossier->bfirst->bdate), &(uptr->FBD)) != 0);
}

SPWAWLIB_API bool	
SPWAW_UHT_is_decommissioned (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	SPWAW_BATTLE *bptr = dossier_find_battle (uptr->uht->dossier, bdate);

	return ((SPWAW_bdate_cmp(bdate, &(uptr->LBD)) == 0) && !uptr->next && (bptr != uptr->uht->dossier->blast));
}

SPWAWLIB_API bool
SPWAW_UHT_is_decommissioned (SPWAW_UHTE *uhte)
{
	SPWAW_UHTE *uptr = SPWAW_UHT_last (uhte);
	if (!uptr) return (true);

	return (SPWAW_bdate_cmp (&(uptr->uht->dossier->blast->bdate), &(uptr->LBD)) != 0);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_first (SPWAW_UHTE *uhte)
{
	if (!uhte) return (NULL);

	while (uhte->prev) uhte = uhte->prev;
	return (uhte);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_last (SPWAW_UHTE *uhte)
{
	if (!uhte) return (NULL);

	while (uhte->next) uhte = uhte->next;
	return (uhte);
}

SPWAWLIB_API bool
SPWAW_UHT_is (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate, USHORT status)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	if (status & UHT_COMMISSIONED) {
		return (SPWAW_UHT_is_commissioned(uhte, bdate));
	}

	if (status & UHT_DECOMMISSIONED) {
		return (SPWAW_UHT_is_decommissioned(uhte, bdate));
	}

	return ((SPWAW_bdate_cmp(bdate, &(uptr->FBD)) == 0) && (uptr->status & status));
}

SPWAWLIB_API bool
SPWAW_UHT_has (SPWAW_UHTE *uhte, USHORT status)
{
	if (!uhte) return (false);

	if (status & UHT_COMMISSIONED) {
		return (SPWAW_UHT_is_commissioned(uhte));
	}

	if (status & UHT_DECOMMISSIONED) {
		return (SPWAW_UHT_is_decommissioned(uhte));
	}

	return (SPWAW_UHT_first (uhte, status) != NULL);
}

SPWAWLIB_API bool
SPWAW_UHT_is (SPWAW_UHTE *uhte, USHORT status)
{
	if (!uhte) return (false);

	if (status & UHT_COMMISSIONED) {
		return (SPWAW_UHT_is_commissioned(uhte));
	}

	if (status & UHT_DECOMMISSIONED) {
		return (SPWAW_UHT_is_decommissioned(uhte));
	}

	return ((uhte->status & status) != 0);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_first (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr;

	if (status & UHT_COMMISSIONED) {
		SPWAW_UHTE *uptr = SPWAW_UHT_first (uhte);
		return (SPWAW_UHT_is_commissioned(uptr)?uptr:NULL);
	}

	if (status & UHT_DECOMMISSIONED) {
		SPWAW_UHTE *uptr = SPWAW_UHT_last (uhte);
		return (SPWAW_UHT_is_decommissioned(uptr)?uptr:NULL);
	}

	uptr = SPWAW_UHT_first (uhte);

	while (uptr && !(uptr->status & status)) uptr = uptr->next;
	return (uptr);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_last (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr;

	if (status & UHT_COMMISSIONED) {
		SPWAW_UHTE *uptr = SPWAW_UHT_first (uhte);
		return (SPWAW_UHT_is_commissioned(uptr)?uptr:NULL);
	}

	if (status & UHT_DECOMMISSIONED) {
		SPWAW_UHTE *uptr = SPWAW_UHT_last (uhte);
		return (SPWAW_UHT_is_decommissioned(uptr)?uptr:NULL);
	}

	uptr = SPWAW_UHT_last (uhte);

	while (uptr && !(uptr->status & status)) uptr = uptr->prev;
	return (uptr);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_next (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr = uhte->next;

	if (status & (UHT_COMMISSIONED|UHT_DECOMMISSIONED)) return (NULL);

	while (uptr && !(uptr->status & status)) uptr = uptr->next;
	return (uptr);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_prev (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr = uhte->prev;

	if (status & (UHT_COMMISSIONED|UHT_DECOMMISSIONED)) return (NULL);

	while (uptr && !(uptr->status & status)) uptr = uptr->prev;
	return (uptr);
}
