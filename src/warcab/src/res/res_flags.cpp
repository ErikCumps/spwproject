/** \file
 * The SPWaW war cabinet - resource handling - nation flags.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "../common.h"

typedef unsigned long(* OOB_FLAG_cb)(void);

typedef struct s_OOB_FLAG_MAP {
	const char		*flagid;
	bool			usecb;
	union u_flag {
		unsigned long	rid;
		OOB_FLAG_cb	cb;
	}			flag;
} OOB_FLAG_MAP;

#define	OOBFLAGID(id_)		{ #id_, false, RID_OOB_FLAG_##id_ }
#define OOBFLAGCB(id_,cb_)	{ #id_, true, (unsigned long)cb_ }

static unsigned long
oob_flag_german_cross_spwaw (void)
{
	return (CFG_german_cross() ? RID_OOB_FLAG_SPWAW_GE_cross : RID_OOB_FLAG_SPWAW_GE_histo);
}

static unsigned long
oob_flag_german_cross_spww2 (void)
{
	return (CFG_german_cross() ? RID_OOB_FLAG_SPWW2_GE_cross : RID_OOB_FLAG_SPWW2_GE_histo);
}

static const OOB_FLAG_MAP flagmap[] = {
	/*** SPWAW flags ***/
	OOBFLAGID(SPWAW_CC),
	OOBFLAGID(SPWAW_CS),
	OOBFLAGID(SPWAW_PI),
	OOBFLAGID(SPWAW_NS),
	OOBFLAGID(SPWAW_RS),
	OOBFLAGID(SPWAW_BU),
	OOBFLAGCB(SPWAW_GE, oob_flag_german_cross_spwaw),
	OOBFLAGID(SPWAW_FI),
	OOBFLAGID(SPWAW_IT),
	OOBFLAGID(SPWAW_RO),
	OOBFLAGID(SPWAW_HU),
	OOBFLAGID(SPWAW_JA),
	OOBFLAGID(SPWAW_FR),
	OOBFLAGID(SPWAW_BR),
	OOBFLAGID(SPWAW_BE),
	OOBFLAGID(SPWAW_NL),
	OOBFLAGID(SPWAW_PO),
	OOBFLAGID(SPWAW_SO),
	OOBFLAGID(SPWAW_US),
	OOBFLAGID(SPWAW_USMC),
	OOBFLAGID(SPWAW_NC),
	OOBFLAGID(SPWAW_GK),
	OOBFLAGID(SPWAW_NOR),
	OOBFLAGID(SPWAW_YU),
	OOBFLAGID(SPWAW_CA),
	OOBFLAGID(SPWAW_IN),
	OOBFLAGID(SPWAW_ANZ),
	OOBFLAGID(SPWAW_FF),

	/*** SPWW2 flags ***/
	OOBFLAGID(SPWW2_ANZ),
	OOBFLAGID(SPWW2_BE),
	OOBFLAGID(SPWW2_BLU),
	OOBFLAGID(SPWW2_BR),
	OOBFLAGID(SPWW2_BU),
	OOBFLAGID(SPWW2_CA),
	OOBFLAGID(SPWW2_CC),
	OOBFLAGID(SPWW2_CS),
	OOBFLAGID(SPWW2_FF),
	OOBFLAGID(SPWW2_FI),
	OOBFLAGID(SPWW2_FR),
	OOBFLAGCB(SPWW2_GE, oob_flag_german_cross_spww2),
	OOBFLAGID(SPWW2_GE_emp),
	OOBFLAGID(SPWW2_GE_old),
	OOBFLAGID(SPWW2_GE_wmr),
	OOBFLAGID(SPWW2_GK),
	OOBFLAGID(SPWW2_GRN),
	OOBFLAGID(SPWW2_HU),
	OOBFLAGID(SPWW2_IN),
	OOBFLAGID(SPWW2_ISR),
	OOBFLAGID(SPWW2_IT),
	OOBFLAGID(SPWW2_JA),
	OOBFLAGID(SPWW2_LWP),
	OOBFLAGID(SPWW2_MN),
	OOBFLAGID(SPWW2_NC),
	OOBFLAGID(SPWW2_NL),
	OOBFLAGID(SPWW2_NOR),
	OOBFLAGID(SPWW2_NS),
	OOBFLAGID(SPWW2_NS_old),
	OOBFLAGID(SPWW2_PO),
	OOBFLAGID(SPWW2_RED),
	OOBFLAGID(SPWW2_RO),
	OOBFLAGID(SPWW2_RS),
	OOBFLAGID(SPWW2_RS_old),
	OOBFLAGID(SPWW2_SE),
	OOBFLAGID(SPWW2_SK),
	OOBFLAGID(SPWW2_SO),
	OOBFLAGID(SPWW2_THA),
	OOBFLAGID(SPWW2_USA),
	OOBFLAGID(SPWW2_USMC),
	OOBFLAGID(SPWW2_VFR),
	OOBFLAGID(SPWW2_YU),
	OOBFLAGID(SPWW2_YU_old),

	/*** unknown flag ***/
	{ 0, false, 0 }
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
			if (flagmap[i].usecb) {
				pm = RES_pixmap (flagmap[i].flag.cb());
			} else {
				pm = RES_pixmap (flagmap[i].flag.rid);
			}
		} else {
			pm = RES_pixmap (RID_OOB_FLAG_SPWAW_00);
		}
	}
	return (pm);
}