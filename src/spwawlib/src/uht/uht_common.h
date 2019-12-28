/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UHT_COMMON_H
#define	UHT_COMMON_H	1

#include "dossier/dossier.h"

extern SPWAW_UHTE *	uht_new_element		(SPWAW_UHT *uht);
extern void		uht_commission		(SPWAW_UHT *uht, BIRURR &rr);
extern void		uht_split_commission	(SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr);
extern void		uht_adjust_commission	(SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr);
extern void		uht_decommission	(SPWAW_UHT *uht, BIRURR &rr, SPWAW_BATTLE *db);
extern void		uht_adjust_decommission	(SPWAW_UHT *uht, BIRURR &rr, BIRURR &nrr);
extern void		uht_link		(SPWAW_UHT *uht, BIRURR &frr, BIRURR &trr, USHORT status);
extern void		uht_sort		(SPWAW_UHT *uht);
extern void		uht_sort_on_index	(SPWAW_UHT *uht);

extern void		uht_status_log		(USHORT status, char *buf, int len);

extern USHORT		uht_detect_changes	(BIRURR &orr, BIRURR &nrr, bool rpl);
extern void		uht_set_filter		(BIRURR &rr, BIRURR_FILTER &f);

#endif	/* UHT_COMMON_H */