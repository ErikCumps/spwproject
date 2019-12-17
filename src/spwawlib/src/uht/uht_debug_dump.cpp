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

SPWAW_ERROR
UHT_debug_dump (SPWAW_UHT *uht)
{
	char	buf[16];

	CNULLARG (uht);

	UHTLOG1 ("UHT[0x%8.8x]\n", uht);
	UHTLOG1 ("dossier=0x%8.8x\n", uht->dossier);
	UHTLOG4 ("cnt=%u, len=%u, list=0x%8.8x, smap=0x%8.8x\n", uht->cnt, uht->len, uht->list, uht->smap);

	for (unsigned int i=0; i<uht->cnt; i++) {
		SPWAW_UHTE *uhte = uht->smap[i];
		SPWAW_BDATE(uhte->FBI, FBIdate); SPWAW_BDATE(uhte->LBI, LBIdate);
		UHTLOG2 ("[%5u] 0x%8.8x ",
			i, uhte);
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
			UHTLOG5 ("------> 0x%8.8x lname=\"%-16s\", UID=%-4s, uname=\"%-16s\", rank=%-3s\n",
				uhte, uhte->lname, uhte->UID, uhte->uname, SPWAW_rank2str(uhte->rank));
		}
	}

	return (SPWERR_OK);
}
