/** \file
 * The SPWaW Library - gamefile handling - winSPWW2 game data.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamedata.h"
#include "gamefile/winspww2/gamedata_winspww2.h"
#include "gamefile/fulist.h"
#include "utils/ud.h"
#include "common/internal.h"
#include "snapshot/index.h"

static SPWAW_ERROR
setup (SPWAW_SNAP_OOB_PRAW *dst, ULIST &up)
{
	SPWAW_SNAP_OOB_PELRAW	*p;

	p = safe_nmalloc (SPWAW_SNAP_OOB_PELRAW, up.cnt); COOM (p, "SPWAW_SNAP_OOB_PELRAW list");
	dst->cnt = up.cnt;
	dst->raw = p;

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_unitpos (WINSPWW2_UNIT_POS *src, USHORT id, SPWAW_SNAP_OOB_PELRAW *dst, USHORT *idx)
{
	SPWAW_SNAP_OOB_PELRAW	*ptr;

	ptr = &(dst[*idx]);

	ptr->RID	= id;
	ptr->URID	= ptr->RID;
	ptr->x		= src->x;
	ptr->y		= src->y;
	ptr->seen	= src->seen;

	if (cfg.withUD) {
		SPWAW_UD *UD = &(ptr->UD);
		UD_init (UD, sizeof (*src));
		UD_ADD (UD, src, __data00);
		UD_ADD (UD, src, __data01);
	}

	(*idx)++;
	return (SPWERR_OK);
}

SPWAW_ERROR
section17_winspww2_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/, FULIST &ful1, FULIST &ful2)
{
	SPWAW_ERROR		rc;
	WINSPWW2_UNIT_POS	*data;
	USHORT			idx;
	UEL			*p;

	CNULLARG (src); CNULLARG (dst);

	data = GDWINSPWW2(src)->sec17.u.d.pos;

	rc = setup (&(dst->raw.OOBp1.positions), ful1.ul); ROE ("setup(struct17/OOBp1)");
	p = ful1.ul.head; idx = 0;
	while (p) {
		rc = add_unitpos (&(data[p->d.RID]), p->d.RID, dst->raw.OOBp1.positions.raw, &idx);
		ROE ("add_unitpos(OOBp1)");
		p = p->l.next;
	}
	rc = build_pridx (&(dst->raw.OOBp1.positions)); ROE ("build_pridx(OOBp1)");

	rc = setup (&(dst->raw.OOBp2.positions), ful2.ul); ROE ("setup(struct17/OOBp2)");
	p = ful2.ul.head; idx = 0;
	while (p) {
		rc = add_unitpos (&(data[p->d.RID]), p->d.RID, dst->raw.OOBp2.positions.raw, &idx);
		ROE ("add_unitpos(OOBp2)");
		p = p->l.next;
	}
	rc = build_pridx (&(dst->raw.OOBp2.positions)); ROE ("build_pridx(OOBp2)");

	return (SPWERR_OK);
}
