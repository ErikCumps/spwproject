/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include "uht/uht.h"
#include "uht/uht_common.h"
#include "common/internal.h"

#if UHTDBGDUMP

void
UHT_debug_dump (SPWAW_UHT *uht)
{
	char	buf[16];

	UHTLOG1 ("SPWAW_UHT[0x%8.8x]\n", uht);
	if (!uht) return;

	UHTLOG1 ("dossier=0x%8.8x\n", uht->dossier);
	UHTLOG4 ("UHT elements: cnt=%u, len=%u, list=0x%8.8x, smap=0x%8.8x\n", uht->cnt, uht->len, uht->list, uht->smap);
	UHTLOG0 ("======================================================================\n");

	for (unsigned int i=0; i<uht->cnt; i++) {
		SPWAW_UHTE *uhte = uht->smap[i];

		UHTLOG2 ("[%5u] 0x%8.8x ", i, uhte);

		SPWAW_BDATE(uhte->FBI, FBIdate); SPWAW_BDATE(uhte->LBI, LBIdate);
		UHTLOG4 ("lname=\"%-16s\", UID=%-4s, uname=\"%-16s\", rank=%-3s, ",
			uhte->lname, uhte->UID, uhte->uname, SPWAW_rank2str(uhte->rank));
		UHTLOG3 ("F=[%s:%05u], L=[%s], ",
			FBIdate, uhte->FUI, LBIdate);
		uht_status_log (uhte->status, buf, sizeof(buf));
		UHTLOG2 ("status=0x%4.4x (%s), ",
			uhte->status, buf);
		UHTLOG2 ("prev=0x%8.8x, next=0x%8.8x\n",
			uhte->prev, uhte->next);

		if (uhte->next) {
			SPWAW_UHTE *uhte = uht->smap[i]->next;
			UHTLOGGED(uhte);

			UHTLOG5 ("------> 0x%8.8x lname=\"%-16s\", UID=%-4s, uname=\"%-16s\", rank=%-3s\n",
				uhte, uhte->lname, uhte->UID, uhte->uname, SPWAW_rank2str(uhte->rank));
		}
	}
	UHTLOG0 ("======================================================================\n");

	UHTLOG3 ("UHT battle info list: cnt=%u, len=%u, list=0x%8.8x\n", uht->blist.cnt, uht->blist.len, uht->blist.info);
	for (unsigned int i=0; i<uht->blist.cnt; i++) {
		SPWAW_UHT_BINFO *info = uht->blist.info[i];
		SPWAW_BDATE(info->bdate, bdate);
		UHTTRACE5 ("[%5u] 0x%8.8x [%s] cnt=%u, list=0x%8.8x\n", i, info, bdate, info->cnt, info->list);

		for (USHORT j=0; j<info->cnt; j++) {
			SPWAW_UHTE *uhte = info->list[j];

			UHTTRACE2 ("    [%5u] 0x%8.8x ", j, uhte);
			if (!uhte) continue;

			SPWAW_BDATE(uhte->FBI, FBIdate); SPWAW_BDATE(uhte->LBI, LBIdate);
			UHTTRACE4 ("lname=\"%-16s\", UID=%-4s, uname=\"%-16s\", rank=%-3s, ",
				uhte->lname, uhte->UID, uhte->uname, SPWAW_rank2str(uhte->rank));
			UHTTRACE3 ("F=[%s:%05u], L=[%s], ",
				FBIdate, uhte->FUI, LBIdate);
			uht_status_log (uhte->status, buf, sizeof(buf));
			UHTTRACE2 ("status=0x%4.4x (%s), ",
				uhte->status, buf);
			UHTTRACE2 ("prev=0x%8.8x, next=0x%8.8x\n",
				uhte->prev, uhte->next);
		}
		UHTLOG0 ("----------------------------------------------------------------------\n");
	}
}

void
UHT_debug_dump (SPWAW_UHT_BINFO *info)
{

	char	buf[16];

	UHTLOG1 ("SPWAW_UHT_BINFO[0x%8.8x]\n", info);
	if (!info) return;

	SPWAW_BDATE(info->bdate, bdate);
	UHTLOG3 ("[%s] cnt=%u, list=0x%8.8x\n", bdate, info->cnt, info->list);

	for (USHORT i=0; i<info->cnt; i++) {
		SPWAW_UHTE *uhte = info->list[i];
		UHTLOG2 ("[%5u] 0x%8.8x ", i, uhte);
		if (!uhte) continue;

		SPWAW_BDATE(uhte->FBI, FBIdate); SPWAW_BDATE(uhte->LBI, LBIdate);
		UHTLOG4 ("lname=\"%-16s\", UID=%-4s, uname=\"%-16s\", rank=%-3s, ",
			uhte->lname, uhte->UID, uhte->uname, SPWAW_rank2str(uhte->rank));
		UHTLOG3 ("F=[%s:%05u], L=[%s], ",
			FBIdate, uhte->FUI, LBIdate);
		uht_status_log (uhte->status, buf, sizeof(buf));
		UHTLOG2 ("status=0x%4.4x (%s), ",
			uhte->status, buf);
		UHTLOG2 ("prev=0x%8.8x, next=0x%8.8x\n",
			uhte->prev, uhte->next);
	}
}

#else  /* !UHTDBGDUMP */

void UHT_debug_dump (SPWAW_UHT *uht) { UNREFERENCED_PARAMETER (uht); }
void UHT_debug_dump (SPWAW_UHT_BINFO *info) { UNREFERENCED_PARAMETER (info); }

#endif /* !UHTDBGDUMP */