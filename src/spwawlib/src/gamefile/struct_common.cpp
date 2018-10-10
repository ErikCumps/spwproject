/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

/* NOTE: none of the FORMATION and UNIT list handling code is thread-safe! */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "common/internal.h"

SPWAW_ERROR
load_from_game (GAMEDATA *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc;
	STRTAB		*stab = NULL;
	FULIST		ful1, ful2;

	CNULLARG (src); CNULLARG (dst);
	stab = (STRTAB *)(dst->stab);

	dst->raw.game.cmt.title = azstrstab (src->cmt.title, stab);
	//dst->raw.game.cmt.mapsrc = azstrstab (src->cmt.mapsrc, stab);

	rc = sec38_save_snapshot (src, dst, stab);		ROE ("sec38_save_snapshot()");
	rc = sec39_save_snapshot (src, dst, stab);		ROE ("sec39_save_snapshot()");
	rc = sec37_save_snapshot (src, dst, stab);		ROE ("sec37_save_snapshot()");
	rc = sec08_save_snapshot (src, dst, stab);		ROE ("sec08_save_snapshot()");
	rc = sec14_save_snapshot (src, dst, stab);		ROE ("sec14_save_snapshot()");

	init_FULIST (ful1);
	init_FULIST (ful2);

	rc = sec35_detection (src, ful1, ful2);			ROE ("sec35_detection()");
	rc = sec01_detection (src, dst, ful1, ful2);		ROE ("sec01_detection()");
	rc = sec35_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec35_save_snapshot()");
	rc = sec01_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec01_save_snapshot()");
	rc = sec34_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec34_save_snapshot()");
	rc = sec17_save_snapshot (src, dst, stab, ful1, ful2);	ROE ("sec17_save_snapshot()");

	rc = sec26_save_snapshot (src, dst, stab);		ROE ("sec26_save_snapshot()");

	return (SPWERR_OK);
}



/* Initializes a unit list */
void
init_ULIST (ULIST &ul)
{
	memset (&ul, 0, sizeof(ul));
}

/* Dumps a unit list */
void
dump_ULIST (ULIST &ul)
{
	UEL	*p, *uel;

	UFDTRACE2 ("ULIST: { { s.nidx=%hu }, cnt=%hu }\n", ul.s.nidx, ul.cnt);

	p = ul.head;
	while (p)
	{
		uel = p; p = p->l.next;

		dump_UEL (uel, uel->d.formation ? "    " : "!!! ");
	}
}

/* Dumps a unit element */
void
dump_UEL (UEL *uel, char *prefix)
{
	UFDTRACE1 ("%s", prefix ? prefix : "    ");

	UFDTRACE3 ("[%5.5u] (%16.16s) %4.4s",
		uel->d.RID, uel->d.name, SPWAW_unittype2str(uel->d.type));
	UFDTRACE2 (" leader=[%5.5u] abandoned=%d",
		uel->d.LRID, uel->d.aband);
	UFDTRACE3 (" loader<%5.5u> vrf=%s vrfneed=%s ",
		uel->d.loader, uel->d.vrfloader ? "Y" : "N", uel->d.needvrfldrtst ? "Y" : "N");
	UFDTRACE2 (" OOB %3.3u: [%3.3u]",
		uel->d.OOB, uel->d.OOBrid);
	UFDTRACE5 (" F[%5.5u]<%5.5u,%5.5u> %s[%5.5u]",
		uel->d.FRID, uel->d.FMID, uel->d.FSID,
		uel->d.formation ? "ptr" : "nil", uel->d.formation ? uel->d.formation->d.RID : SPWAW_BADIDX);
	UFDTRACE4 (" link crew=%s[%5.5u], parent=%s[%5.5u]\n",
		uel->d.link.crew ? "ptr" : "nil", uel->d.link.crew ? uel->d.link.crew->d.RID : SPWAW_BADIDX,
		uel->d.link.parent ? "ptr" : "nil", uel->d.link.parent ? uel->d.link.parent->d.RID : SPWAW_BADIDX);
}

/* Reserves the next available element for the unit list */
UEL *
reserve_UEL (ULIST &ul)
{
	UEL *uel = NULL;

	if (ul.s.nidx < UNITCOUNT) {
		uel = &(ul.s.list[ul.s.nidx]);

		uel->l.prev = uel->l.next = NULL;

		memset (&(uel->d), 0, sizeof (uel->d));

		uel->d.OOBtype = SPWOOB_UTYPE__NONE;
		uel->d.type = SPWAW_UNIT_TYPE_UNIT;
	}

	return (uel);
}

/* Commits the (currently reserved) element to the unit list */
bool
commit_UEL (ULIST &ul, UEL *uel)
{
	UEL	*prev;

	if (uel != &(ul.s.list[ul.s.nidx])) return (false);

	if (ul.s.nidx > 0) {
		prev = &(ul.s.list[ul.s.nidx-1]);
		prev->l.next = uel;
		uel->l.prev = prev;
	}

	if (!ul.head) ul.head = uel;

	ul.s.nidx++;
	ul.cnt++;

	return (true);
}

/* Drops the element from the unit list */
void
drop_UEL (ULIST &ul, UEL *uel)
{
	if (ul.cnt == 0) return;

	if (uel->l.prev) {
		uel->l.prev->l.next = uel->l.next;
	} else {
		ul.head = uel->l.next;
	}
	if (uel->l.next) {
		uel->l.next->l.prev = uel->l.prev;
	}
	ul.cnt--;

	if ((uel->d.type == SPWAW_UNIT_TYPE_UNIT) && uel->d.link.crew) {
		drop_UEL (ul, uel->d.link.crew);
	}
}

/* Looks up the element in the unit list for the given unit record ID */
UEL *
lookup_ULIST (ULIST &ul, USHORT urid)
{
	UEL	*p = ul.head;

	while (p) {
		if (urid == p->d.RID) break;
		p = p->l.next;
	}

	return (p);
}



/* Initializes a formation list */
void
init_FLIST (FLIST &fl)
{
	memset (&fl, 0, sizeof(fl));
}

/* Dumps a formation list */
void
dump_FLIST (FLIST &fl)
{
	FEL	*p, *fel;

	UFDTRACE2 ("FLIST: { { s.nidx=%hu }, cnt=%hu }\n", fl.s.nidx, fl.cnt);

	p = fl.head;
	while (p)
	{
		fel = p; p = fel->l.next;

		dump_FEL (fel, "    ");
	}
}

void
dump_FEL (FEL *fel, char *prefix)
{
	UFDTRACE1 ("%s", prefix ? prefix : "    ");

	UFDTRACE6 ("[%5.5u] (%16.16s) P<%3.3u> L<%5.5u> status=%3.3u units=%3.3u",
		fel->d.RID, fel->d.name, fel->d.player, fel->d.leader, fel->d.status, fel->d.unit_cnt);
	UFDTRACE4 (" OOB %3.3u: [%3.3u] FID<%5.5u> RAW<%5.5u>\n",
		fel->d.OOB, fel->d.OOBrid, fel->d.FID, fel->d.rawFID);
	for (BYTE i=0; i<fel->d.unit_cnt; i++) {
		UFDTRACE4 ("    %s [%3.3u] %s[%5.5u]\n",
			fel->d.unit_lst[i] ? "   " : "---",
			i,
			fel->d.unit_lst[i] ? "ptr" : "nil",
			fel->d.unit_lst[i] ? fel->d.unit_lst[i]->d.RID : SPWAW_BADIDX);
	}
}
/* Reserves the next available element for the formation list */
FEL *
reserve_FEL (FLIST &fl)
{
	FEL	*fel = NULL;

	if (fl.s.nidx < FORMCOUNT) {
		fel = &(fl.s.list[fl.s.nidx]);

		fel->l.prev = fel->l.next = NULL;

		memset (&(fel->d), 0, sizeof (fel->d));
	}

	return (fel);
}

/* Commits the (currently reserved) element to the formation list */
bool
commit_FEL (FLIST &fl, FEL *fel)
{
	FEL	*prev;

	if (fel != &(fl.s.list[fl.s.nidx])) return (false);

	if (fl.s.nidx > 0) {
		prev = &(fl.s.list[fl.s.nidx-1]);
		prev->l.next = fel;
		fel->l.prev = prev;
	}

	if (!fl.head) fl.head = fel;

	// Compensate for raw formation IDs that are offset by a multiple of FORMPCOUNT
	fel->d.FID = (fel->d.rawFID % FORMPCOUNT) - (fl.head->d.rawFID % FORMPCOUNT);

	fl.s.nidx++;
	fl.cnt++;

	return (true);
}

/* Drops the element from the formation list */
void
drop_FEL (FLIST &fl, FEL *fel)
{
	if (fl.cnt == 0) return;

	if (fel->l.prev) {
		fel->l.prev->l.next = fel->l.next;
	} else {
		fl.head = fel->l.next;
	}
	if (fel->l.next) {
		fel->l.next->l.prev = fel->l.prev;
	}
	fl.cnt--;
}

/* Looks up the element in the formation list for the given formation record ID */
FEL *
lookup_FLIST (FLIST &fl, USHORT frid)
{
	FEL	*p = fl.head;

	while (p) {
		if (frid == p->d.RID) break;
		p = p->l.next;
	}

	return (p);
}


/* Associates a crew with its parent unit */
bool
add_crew_to_unit (UEL *cel, UEL *uel)
{
	if (!cel || !uel) return (false);

	if (uel->d.type == SPWAW_UNIT_TYPE_CREW) return (false);

	uel->d.link.crew = cel;

	cel->d.type = SPWAW_UNIT_TYPE_CREW;
	cel->d.link.parent = uel;

	return (true);
}

/* Associates a unit with its formation */
bool
add_unit_to_formation (FEL *fel, UEL *uel)
{
	if (!fel || !uel) return (false);

	if (uel->d.FSID >= MAXFORMATIONUNITS) {
		log ("failed to add unit to formation: max unit count (%u) reached\n", MAXFORMATIONUNITS);
		return (false);
	}
	if (fel->d.unit_lst[uel->d.FSID]) {
		log ("failed to add unit to formation: duplicate unit FSID (%u) detected\n", uel->d.FSID);
		return (false);
	}

	fel->d.unit_lst[uel->d.FSID] = uel;
	fel->d.unit_cnt++;
	uel->d.formation = fel;

	return (true);
}


/* Initializes a combined formation/unit list */
void
init_FULIST (FULIST &l)
{
	init_FLIST (l.fl);
	init_ULIST (l.ul);
}

/* Dumps a combined formation/unit list */
void
dump_FULIST (FULIST &l)
{
	dump_FLIST (l.fl);
	dump_ULIST (l.ul);
}
