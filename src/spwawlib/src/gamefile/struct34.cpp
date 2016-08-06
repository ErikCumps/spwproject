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
setup (SPWAW_SNAP_OOB_LRAW *dst, UNIT_LIST &up)
{
	SPWAW_SNAP_OOB_LELRAW	*p;

	p = safe_nmalloc (SPWAW_SNAP_OOB_LELRAW, up.cnt); COOM (p, "SPWAW_SNAP_OOB_LELRAW list");
	dst->cnt = up.cnt;
	dst->raw = p;

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_leader (LEADER *src, USHORT id, SPWAW_SNAP_OOB_LELRAW *dst, USHORT *idx, STRTAB *stab)
{
	SPWAW_SNAP_OOB_LELRAW	*ptr;
	SPWAW_UD		*UD;

	ptr = &(dst[*idx]);

	ptr->RID	= id;
	ptr->URID	= src->UID;
	ptr->name = azstrstab (src->name, stab);
	ptr->rank	= src->rank;
	ptr->ral	= src->rally;
	ptr->inf	= src->skinf;
	ptr->art	= src->skart;
	ptr->arm	= src->skarm;
	ptr->kills	= src->kills;
	ptr->status	= src->status;

	if (cfg.withUD) {
		UD = &(ptr->UD);
		UD_init (UD, sizeof (*src));
		UD_ADD (UD, src, __data00);
		UD_ADD (UD, src, __data01);
		UD_ADD (UD, src, __data02);
		UD_ADD (UD, src, __data03);
	}

	(*idx)++;
	return (SPWERR_OK);
}

SPWAW_ERROR
sec34_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, UNIT_LIST &ul1, UNIT_LIST &ul2)
{
	SPWAW_ERROR	rc;
	LEADER		*data;
	USHORT		i, idx;

	CNULLARG (src); CNULLARG (dst);

	data = src->sec34.u.d.leaders;

	rc = setup (&(dst->raw.OOBp1.leaders), ul1); ROE ("setup(struct34/OOBp1)");
	for (idx = i = 0; i<ul1.cnt; i++) {
		rc = add_leader (&(data[ul1.list[i]]), ul1.list[i], dst->raw.OOBp1.leaders.raw, &idx, stab);
		ROE ("add_leader(OOBp1)");
	}
	rc = build_lridx (&(dst->raw.OOBp1.leaders)); ROE ("build_lridx(OOBp1)");

	rc = setup (&(dst->raw.OOBp2.leaders), ul2); ROE ("setup(struct34/OOBp2)");
	for (idx = i = 0; i<ul2.cnt; i++) {
		rc = add_leader (&(data[ul2.list[i]]), ul2.list[i], dst->raw.OOBp2.leaders.raw, &idx, stab);
		ROE ("add_leader(OOBp2)");
	}
	rc = build_lridx (&(dst->raw.OOBp2.leaders)); ROE ("build_lridx(OOBp2)");

	return (SPWERR_OK);
}
