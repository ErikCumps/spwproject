#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "strtab/strtab.h"
#include "common/internal.h"

static void
fixup_snap (SPWAW_SNAPSHOT *src, STRTAB *stab)
{
	src->src.path = STRTAB_getstr (stab, STRTAB_getidx (stab, src->src.path));
	src->src.file = STRTAB_getstr (stab, STRTAB_getidx (stab, src->src.file));

	src->raw.game.cmt.title = STRTAB_getstr (stab, STRTAB_getidx (stab, src->raw.game.cmt.title));
	src->raw.game.cmt.mapsrc = STRTAB_getstr (stab, STRTAB_getidx (stab, src->raw.game.cmt.mapsrc));
	src->raw.game.battle.location = STRTAB_getstr (stab, STRTAB_getidx (stab, src->raw.game.battle.location));
}

static void
fixup_roob (SPWAW_SNAP_OOB_RAW *roob, STRTAB *stab)
{
	DWORD	i;

	for (i=0; i<roob->formations.cnt; i++) {
		roob->formations.raw[i].name = STRTAB_getstr (stab, STRTAB_getidx (stab, roob->formations.raw[i].name));
	}
	for (i=0; i<roob->leaders.cnt; i++) {
		roob->leaders.raw[i].name = STRTAB_getstr (stab, STRTAB_getidx (stab, roob->leaders.raw[i].name));
	}
	for (i=0; i<roob->units.cnt; i++) {
		roob->units.raw[i].name = STRTAB_getstr (stab, STRTAB_getidx (stab, roob->units.raw[i].name));
	}
}

static void
fixup_foob (SPWAW_SNAP_OOB_FORCE *foob, STRTAB *stab)
{
	DWORD	i;

	for (i=0; i<foob->formations.cnt; i++) {
		foob->formations.list[i].strings.utype = STRTAB_getstr (stab, STRTAB_getidx (stab, foob->formations.list[i].strings.utype));
	}
	for (i=0; i<foob->units.cnt; i++) {
		foob->units.list[i].data.type = STRTAB_getstr (stab, STRTAB_getidx (stab, foob->units.list[i].data.type));
		foob->units.list[i].data.name = STRTAB_getstr (stab, STRTAB_getidx (stab, foob->units.list[i].data.name));
	}
}

static void
fixup_oob (SPWAW_SNAP_OOB_RAW *roob, SPWAW_SNAP_OOB *oob, STRTAB *stab)
{
	fixup_roob (roob,		stab);
	fixup_foob (&(oob->battle),	stab);
	fixup_foob (&(oob->core),	stab);
	fixup_foob (&(oob->support),	stab);
}

SPWAW_ERROR
snapstabmerge (SPWAW_SNAPSHOT *snap, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	//{
	//	FILE	*rf = fopen ("STRTAB_report.txt", "a");
	//	debug_STRTAB_report (rf, (STRTAB *)snap->stab, "(snap) before STRTAB_merge");
	//	debug_STRTAB_report (rf, stab, "(stab) before STRTAB_merge");
	//	fclose (rf);
	//}

	rc = STRTAB_merge (stab, (STRTAB *)snap->stab);
	ROE ("STRTAB_merge()");

	//{
	//	FILE	*rf = fopen ("STRTAB_report.txt", "a");
	//	debug_STRTAB_report (rf, stab, "after STRTAB_merge");
	//	fclose (rf);
	//}

	fixup_snap (snap, stab);
	fixup_oob (&(snap->raw.OOBp1), &(snap->OOBp1), stab);
	fixup_oob (&(snap->raw.OOBp2), &(snap->OOBp2), stab);

	STRTAB_free ((STRTAB **)&(snap->stab));
	snap->stab = stab;

	return (SPWERR_OK);
}

static void
remove_snap (SPWAW_SNAPSHOT *src, STRTAB *stab)
{
	STRTAB_del (stab, src->src.path);
	STRTAB_del (stab, src->src.file);

	STRTAB_del (stab, src->raw.game.cmt.title);
	STRTAB_del (stab, src->raw.game.cmt.mapsrc);
	STRTAB_del (stab, src->raw.game.battle.location);
}

static void
remove_oobf (SPWAW_SNAP_OOB_FELRAW *src, STRTAB *stab)
{
	STRTAB_del (stab, src->name);
}

static void
remove_oobl (SPWAW_SNAP_OOB_LELRAW *src, STRTAB *stab)
{
	STRTAB_del (stab, src->name);
}

static void
remove_oobu (SPWAW_SNAP_OOB_UELRAW *src, STRTAB *stab)
{
	STRTAB_del (stab, src->name);
}

static void
remove_oob (SPWAW_SNAP_OOB_RAW *roob, STRTAB *stab)
{
	ULONG	i;

	for (i=0; i<roob->formations.cnt; i++) {
		remove_oobf (&(roob->formations.raw[i]), stab);
	}
	for (i=0; i<roob->leaders.cnt; i++) {
		remove_oobl (&(roob->leaders.raw[i]), stab);
	}
	for (i=0; i<roob->units.cnt; i++) {
		remove_oobu (&(roob->units.raw[i]), stab);
	}
}

SPWAW_ERROR
snapstabremove (SPWAW_SNAPSHOT *snap, STRTAB *stab)
{
	remove_snap (snap, stab);
	remove_oob (&(snap->raw.OOBp1), stab);
	remove_oob (&(snap->raw.OOBp2), stab);

	//{
	//	FILE	*rf = fopen ("STRTAB_report.txt", "a");
	//	debug_STRTAB_report (rf, stab, "after snapshot STRTAB_del");
	//	fclose (rf);
	//}

	return (SPWERR_OK);
}
