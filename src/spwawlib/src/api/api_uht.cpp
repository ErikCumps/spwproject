/** \file
 * The SPWaW Library - unit history tracking API implementation.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
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

	while ((SPWAW_bdate_cmp(bdate, &(u->LBI)) > 0) && u->next) { u = u->next; }
	return (u);
}

SPWAWLIB_API SPWAW_DOSSIER_UIR*
SPWAW_UHT_lookup (SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate)
{
	USHORT			fui;
	SPWAW_BATTLE_DATE	fbi;

	SPWAW_UHTE *uptr = uht_lookup (base, bdate);

	if ((SPWAW_bdate_cmp(bdate, &(uptr->FBI)) < 0) || (SPWAW_bdate_cmp(&(uptr->LBI), bdate) < 0))
		return (NULL);

	SPWAW_BATTLE *bptr = uptr->uht->dossier->bfirst;
	while ((SPWAW_bdate_cmp(&(bptr->bdate), &(uptr->FBI)) != 0) && bptr->next) bptr = bptr->next;
	if (SPWAW_bdate_cmp(&(bptr->bdate), &(uptr->FBI)) != 0) return (NULL);

	fui = uptr->FUI; fbi = uptr->FBI;
	while ((SPWAW_bdate_cmp(&fbi, bdate) < 0) && bptr->next) {
		if (bptr->ra[fui].dst == SPWAW_BADIDX) break;

		fui = bptr->ra[fui].dst; fbi = bptr->next->bdate;
		bptr = bptr->next;
	}
	if (SPWAW_bdate_cmp(&(bptr->bdate), bdate) != 0) return (NULL);

	return (&(bptr->info_sob->pbir_core.uir[fui]));
}

SPWAWLIB_API bool
SPWAW_UHT_is_active (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	return ((SPWAW_bdate_cmp(bdate, &(uptr->FBI)) >= 0) && (SPWAW_bdate_cmp(bdate, &(uptr->LBI)) <= 0));
}

SPWAWLIB_API bool
SPWAW_UHT_is_commissioned (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	return ((SPWAW_bdate_cmp(bdate, &(uptr->FBI)) == 0) && !uptr->prev);
}

SPWAWLIB_API bool	
SPWAW_UHT_is_decommissioned (SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate)
{
	SPWAW_UHTE *uptr = uht_lookup (uhte, bdate);
	if (!uptr) return (false);

	SPWAW_BATTLE *bptr = dossier_find_battle (uptr->uht->dossier, bdate);

	return ((SPWAW_bdate_cmp(bdate, &(uptr->LBI)) == 0) && !uptr->next && (bptr != uptr->uht->dossier->blast));
}

SPWAWLIB_API bool
SPWAW_UHT_is_decommissioned (SPWAW_UHTE *uhte)
{
	SPWAW_UHTE *uptr = SPWAW_UHT_last (uhte);
	if (!uptr) return (true);

	return (SPWAW_bdate_cmp (&(uptr->uht->dossier->blast->bdate), &(uptr->LBI)) != 0);
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

	return ((SPWAW_bdate_cmp(bdate, &(uptr->FBI)) == 0) && (uptr->status & status));
}

SPWAWLIB_API bool
SPWAW_UHT_is (SPWAW_UHTE *uhte, USHORT status)
{
	if (!uhte) return (false);

	return ((uhte->status & status) != 0);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_first (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr = SPWAW_UHT_first (uhte);

	while (uptr && !(uptr->status & status)) uptr = uptr->next;
	return (uptr);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_last (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr = SPWAW_UHT_last (uhte);

	while (uptr && !(uptr->status & status)) uptr = uptr->prev;
	return (uptr);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_next (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr = uhte->next;

	while (uptr && !(uptr->status & status)) uptr = uptr->next;
	return (uptr);
}

SPWAWLIB_API SPWAW_UHTE *
SPWAW_UHT_prev (SPWAW_UHTE *uhte, USHORT status)
{
	SPWAW_UHTE *uptr = uhte->prev;

	while (uptr && !(uptr->status & status)) uptr = uptr->prev;
	return (uptr);
}
