/** \file
 * The SPWaW war cabinet - resource handling - nation flags.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"

typedef struct s_OOB_FLAG_MAP {
	const char	*flagid;
	unsigned long	rid;
} OOB_FLAG_MAP;

#define	OOBFLAG(id_)	{ #id_, RID_OOB_FLAG_##id_ }

static const OOB_FLAG_MAP flagmap[] = {
	/*** SPWAW flags ***/
	OOBFLAG(SPWAW_CC),
	OOBFLAG(SPWAW_CS),
	OOBFLAG(SPWAW_PI),
	OOBFLAG(SPWAW_NS),
	OOBFLAG(SPWAW_RS),
	OOBFLAG(SPWAW_BU),
	OOBFLAG(SPWAW_GE),
	OOBFLAG(SPWAW_FI),
	OOBFLAG(SPWAW_IT),
	OOBFLAG(SPWAW_RO),
	OOBFLAG(SPWAW_HU),
	OOBFLAG(SPWAW_JA),
	OOBFLAG(SPWAW_FR),
	OOBFLAG(SPWAW_BR),
	OOBFLAG(SPWAW_BE),
	OOBFLAG(SPWAW_NL),
	OOBFLAG(SPWAW_PO),
	OOBFLAG(SPWAW_SO),
	OOBFLAG(SPWAW_US),
	OOBFLAG(SPWAW_USMC),
	OOBFLAG(SPWAW_NC),
	OOBFLAG(SPWAW_GK),
	OOBFLAG(SPWAW_NOR),
	OOBFLAG(SPWAW_YU),
	OOBFLAG(SPWAW_CA),
	OOBFLAG(SPWAW_IN),
	OOBFLAG(SPWAW_ANZ),
	OOBFLAG(SPWAW_FF),

	/*** SPWW2 flags ***/
	OOBFLAG(SPWW2_ANZ),
	OOBFLAG(SPWW2_BE),
	OOBFLAG(SPWW2_BLU),
	OOBFLAG(SPWW2_BR),
	OOBFLAG(SPWW2_BU),
	OOBFLAG(SPWW2_CA),
	OOBFLAG(SPWW2_CC),
	OOBFLAG(SPWW2_CS),
	OOBFLAG(SPWW2_FF),
	OOBFLAG(SPWW2_FI),
	OOBFLAG(SPWW2_FR),
	OOBFLAG(SPWW2_GE),
	OOBFLAG(SPWW2_GE_emp),
	OOBFLAG(SPWW2_GE_old),
	OOBFLAG(SPWW2_GE_wmr),
	OOBFLAG(SPWW2_GK),
	OOBFLAG(SPWW2_GRN),
	OOBFLAG(SPWW2_HU),
	OOBFLAG(SPWW2_IN),
	OOBFLAG(SPWW2_ISR),
	OOBFLAG(SPWW2_IT),
	OOBFLAG(SPWW2_JA),
	OOBFLAG(SPWW2_LWP),
	OOBFLAG(SPWW2_MN),
	OOBFLAG(SPWW2_NC),
	OOBFLAG(SPWW2_NL),
	OOBFLAG(SPWW2_NOR),
	OOBFLAG(SPWW2_NS),
	OOBFLAG(SPWW2_NS_old),
	OOBFLAG(SPWW2_PO),
	OOBFLAG(SPWW2_RED),
	OOBFLAG(SPWW2_RO),
	OOBFLAG(SPWW2_RS),
	OOBFLAG(SPWW2_RS_old),
	OOBFLAG(SPWW2_SE),
	OOBFLAG(SPWW2_SK),
	OOBFLAG(SPWW2_SO),
	OOBFLAG(SPWW2_THA),
	OOBFLAG(SPWW2_USA),
	OOBFLAG(SPWW2_USMC),
	OOBFLAG(SPWW2_VFR),
	OOBFLAG(SPWW2_YU),
	OOBFLAG(SPWW2_YU_old),

	/*** unknown flag ***/
	{ 0, 0 }
};

QPixmap *
RES_flag (SPWAW_GAME_TYPE gametype, BYTE oob)
{
	return (RES_flagbyid (SPWAW_oob_flagid (gametype, oob)));
}

QPixmap *
RES_flagbyid (const char *flagid)
{
	int		i = 0;
	QPixmap		*pm = NULL;

	if (!flagid) {
		pm = RES_pixmap (RID_OOB_FLAG_SPWAW_00);
	} else {
		while (flagmap[i].flagid != 0) {
			if (strcmp (flagmap[i].flagid, flagid) == 0) break;
			i++;
		}
		if (flagmap[i].flagid != 0) {
			pm = RES_pixmap (flagmap[i].rid);
		} else {
			pm = RES_pixmap (RID_OOB_FLAG_SPWAW_00);
		}
	}
	return (pm);
}