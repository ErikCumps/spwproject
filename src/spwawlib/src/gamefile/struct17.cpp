/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "snapshot/index.h"
#include "snapshot/snapshot.h"
#include "utils/ud.h"
#include "common/internal.h"

static SPWAW_ERROR
setup (SPWAW_SNAP_OOB_PRAW *dst, UNIT_LIST &up)
{
	SPWAW_SNAP_OOB_PELRAW	*p;

	p = safe_nmalloc (SPWAW_SNAP_OOB_PELRAW, up.cnt); COOM (p, "SPWAW_SNAP_OOB_PELRAW list");
	dst->cnt = up.cnt;
	dst->raw = p;

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_unitpos (UNIT_POS *src, USHORT id, SPWAW_SNAP_OOB_PELRAW *dst, USHORT *idx)
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
sec17_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB * /*stab*/, UNIT_LIST &ul1, UNIT_LIST &ul2)
{
	SPWAW_ERROR	rc;
	UNIT_POS	*data;
	USHORT		i, idx;

	CNULLARG (src); CNULLARG (dst);

	data = src->sec17.u.d.pos;

	rc = setup (&(dst->raw.OOBp1.positions), ul1); ROE ("setup(struct17/OOBp1)");
	for (idx = i = 0; i<ul1.cnt; i++) {
		rc = add_unitpos (&(data[ul1.list[i]]), ul1.list[i], dst->raw.OOBp1.positions.raw, &idx);
		ROE ("add_unitpos(OOBp1)");
	}
	rc = build_pridx (&(dst->raw.OOBp1.positions)); ROE ("build_pridx(OOBp1)");

	rc = setup (&(dst->raw.OOBp2.positions), ul2); ROE ("setup(struct17/OOBp2)");
	for (idx = i = 0; i<ul2.cnt; i++) {
		rc = add_unitpos (&(data[ul2.list[i]]), ul2.list[i], dst->raw.OOBp2.positions.raw, &idx);
		ROE ("add_unitpos(OOBp2)");
	}
	rc = build_pridx (&(dst->raw.OOBp2.positions)); ROE ("build_pridx(OOBp2)");

	return (SPWERR_OK);
}
