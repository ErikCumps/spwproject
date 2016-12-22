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
#include "utils/log.h"
#include "common/internal.h"

// FIXME: the following description is not quite correct!

// FIXME: the end-of-unitlist detection is not 100% correct.
//
// The problem is caused by leftover data from previous battles which
// are still present in the gamefile. This can lead to invalid unit
// counts and invalid unit data.
//
// The algorithm currently stops at the last detected crew member but:
// 	+ the last detected crew member may be invalid leftover data
//	+ valid units may be present after the last crew member (reinforcements?)
//
// Therefore it should be reworked.
//
// What is known:
//	+ units are entered in groups per formation
//	+ for each unit the formation ID must be >= the formation of the last unit
//	+ crews come after their associated units
//	+ crews must have an associated unit

// Actually, the unit records reference the formation records using a formation ID.
// Therefore, a valid unit has a valid name and references a valid formation.
// In addition, because the formation records contain player ownership information,
// it is immediately clear to which player a unit belongs and there is no need to
// perform any checking of the OOB ID.

static inline bool
checkunit (USHORT u, UNIT_LIST &ul)
{
	USHORT	i;

	for (i=0; i<ul.cnt; i++)
		if (u == ul.list[i]) return (true);

	return (false);
}

static inline bool
checkformation (USHORT cf, USHORT csf, USHORT &lf, USHORT &lsf)
{
	if (lf != SPWAW_BADIDX) {
		if (cf < lf) return (false);
		if (cf == lf) {
			if ((lsf != SPWAW_BADIDX) && (csf <= lsf)) {
				lf++; lsf = SPWAW_BADIDX;
				return (false);
			}
		}
	}
	lf = cf;
	lsf = csf;
	return (true);
}

static enum state {
	idle,
	busyf1,
	busyf2,
};

static void
unitcount_oldmethod (UNIT *data, SPWAW_SNAP_OOB_FRAW *fp1, UNIT_LIST &ul1, SPWAW_SNAP_OOB_FRAW *fp2, UNIT_LIST &ul2)
{
	state	s = idle;
	USHORT	i;
	USHORT	u, c;
	USHORT	lf1, lsf1, lf2, lsf2;
	bool	isp1;
	char	name[17];

	ul1.cnt = ul2.cnt = 0;

	lf1 = lf2 = SPWAW_BADIDX; lsf1 = lsf2 = 0;
	for (i=0; i<UNITCOUNT; i++)
	{
		if (data[i].name[0] == '\0') {
			log ("unitcount_oldmethod: [%3.3u] INVALID\n", i);
			switch (s) {
				case busyf1:
				case busyf2:
					if (lf1 != SPWAW_BADIDX) { lf1++; lsf1 = SPWAW_BADIDX; }
					if (lf2 != SPWAW_BADIDX) { lf2++; lsf2 = SPWAW_BADIDX; }
					s = idle;
					break;
				case idle:
				default:
					break;
			}
			continue;
		}
		snprintf (name, sizeof (name) - 1, "%s", data[i].name); name[16] = '\0';

		// Unit or crew?
		if ((data[i].crew == SPWAW_BADIDX) || (data[i].crew > i))
		{
			log ("unitcount_oldmethod: [%3.3u] UNIT: F<%3.3u,%3.3u> C<%5.5u> (%16.16s) ", i, data[i].formID, data[i].minform, data[i].crew, name);

			// A unit must have a valid formation ID,
			// the formation ID must be the same or higher than the last formation ID,
			// the subformation ID must be higher than the last subformation ID,
			// unless this is a new formation ID, in which case the subformation ID must be 0

			// Ideally, the formation oob info should be used to obtain the highest subformation ID,
			// However, sometimes the OOBid field of the formation record is 0!

			// Sadly, we currently cannot filter the following invalid units:
			// in X0, X1, X2, X4, X5
			// with X4, X5 units of X formation in previous battle
			// with current valid X formation containing only 3 units X0, X1, X2

			if (check_formationid (data[i].formID, fp1, NULL)) {
				s = busyf1;
				//if (!checkformation (data[i].formID, data[i].minform, lf1, lsf1)) {
				//	log ("SKIPPED: invalid formation IDs\n");
				//} else {
					log_nots ("OK: #1\n");
					ul1.list[ul1.cnt++] = i;
					lf1 = data[i].formID;
				//}
				continue;
			}
			if (check_formationid (data[i].formID, fp2, NULL)) {
				s = busyf2;
				//if (!checkformation (data[i].formID, data[i].minform, lf2, lsf2)) {
				//	log ("SKIPPED: invalid formation IDs\n");
				//} else {
					log_nots ("OK: #2\n");
					ul2.list[ul2.cnt++] = i;
					lf2 = data[i].formID;
				//}
				continue;
			}
		} else {
			log ("unitcount_oldmethod: [%3.3u] CREW: F<%3.3u,%3.3u> C<%5.5u> (%16.16s) ", i, data[i].formID, data[i].minform, data[i].crew, name);
			s = idle;

			// A crew member must point to a valid unit,
			// the parent unit must have a crew record ID pointing back to the crew member

			u = data[i].crew;
			if (((isp1 = checkunit (u, ul1)) == 0) && !checkunit (u, ul2)) {
				log_nots ("SKIPPED: invalid crew parent unit\n");
				continue;
			}

			c = data[u].crew;
			if (c != i) {
				log_nots ("SKIPPED: invalid crew <> parent unit linkage\n");
				continue;
			}

			if (isp1) {
				log_nots ("OK: #1\n");
				ul1.list[ul1.cnt++] = i;
			} else {
				log_nots ("OK: #2\n");
				ul2.list[ul2.cnt++] = i;
			}
			continue;
		}
		log_nots ("SKIPPED!\n");
	}
	log ("unitcount_oldmethod: ul1.cnt=%3.3u, ul2.cnt=%3.3u\n", ul1.cnt, ul2.cnt);
}

static void
unitcount (UNIT *data, USHORT start, USHORT stop, BYTE player, SPWAW_SNAP_OOB_FRAW *fp, UNIT_LIST &ul)
{
	USHORT	i;
	USHORT	u, c;
	USHORT	lf, lsf;
	char	name[17];
	bool	crew_seen = false;

	ul.cnt = 0;

	lf = SPWAW_BADIDX; lsf = 0;
	for (i=start; i<=stop; i++)
	{
		if (data[i].name[0] == '\0') {
			log ("unitcount: [%3.3u] INVALID\n", i);
			break;
		}
		snprintf (name, sizeof (name) - 1, "%s", data[i].name); name[16] = '\0';

		// Unit or crew?
		if ((data[i].crew == SPWAW_BADIDX) || (data[i].crew > i))
		{
			log ("unitcount: [%3.3u] UNIT: F<%3.3u,%3.3u> C<%5.5u> L<%5.5u> (%16.16s) ", i, data[i].formID, data[i].minform, data[i].crew, data[i].leader, name);

			// There can be no valid units after a block of valid crews?
			if (crew_seen) {
				log_nots ("SKIPPED: UNIT_AFTER_CREW\n");
				break;
			}

			// A unit must have a valid formation ID,
			// the formation ID must be the same or higher than the last formation ID,
			// the subformation ID must be higher than the last subformation ID,
			// unless this is a new formation ID, in which case the subformation ID must be 0

			// Ideally, the formation oob info should be used to obtain the highest subformation ID,
			// However, sometimes the OOBid field of the formation record is 0!

			// Sadly, we currently cannot filter the following invalid units:
			// in X0, X1, X2, X4, X5
			// with X4, X5 units of X formation in previous battle
			// with current valid X formation containing only 3 units X0, X1, X2

			if (check_formationid (data[i].formID, fp, NULL)) {
				//if (!checkformation (data[i].formID, data[i].minform, lf, lsf)) {
				//	log ("SKIPPED: invalid formation IDs\n");
				//	break;
				//} else {
					log_nots ("OK: #%u\n", player);
					ul.list[ul.cnt++] = i;
					lf = data[i].formID;
				//}
				continue;
			}
		} else {
			log ("unitcount: [%3.3u] CREW: F<%3.3u,%3.3u> C<%5.5u> L<%5.5u> (%16.16s) ", i, data[i].formID, data[i].minform, data[i].crew, data[i].leader, name);

			// A crew member must point to a valid unit,
			// the parent unit must have a crew record ID pointing back to the crew member

			u = data[i].crew;
			if (!checkunit (u, ul)) {
				log_nots ("SKIPPED: invalid crew parent unit\n");
				break;
			}

			c = data[u].crew;
			if (c != i) {
				log_nots ("SKIPPED: invalid crew <> parent unit linkage\n");
				break;
			}

			log_nots ("OK: #%u\n", player);
			ul.list[ul.cnt++] = i;
			crew_seen = true;
			continue;
		}
		log_nots ("SKIPPED!\n");
		//break;
	}
	log ("unitcount: ul.cnt=%3.3u\n", ul.cnt);
}

static SPWAW_ERROR
setup (SPWAW_SNAP_OOB_URAW *dst, UNIT_LIST &up)
{
	SPWAW_SNAP_OOB_UELRAW	*p;

	p = safe_nmalloc (SPWAW_SNAP_OOB_UELRAW, up.cnt); COOM (p, "SPWAW_SNAP_OOB_UELRAW list");
	dst->cnt   = up.cnt;
	dst->raw   = p;
	dst->start = up.cnt ? up.list[0] : 0;

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_unit (UNIT *src, USHORT id, SPWAW_SNAP_OOB_UELRAW *dst, USHORT *idx, SPWAW_SNAP_OOB_FRAW *fp, STRTAB *stab)
{
	SPWAW_SNAP_OOB_UELRAW	*ptr;

	ptr = &(dst[*idx]);

	log ("add_unit: idx=%u, RID=%u\n", *idx, id);

	ptr->RID	= id;
	ptr->FRID	= src->formID;
	check_formationid (ptr->FRID, fp, &(ptr->FMID));
	ptr->FSID	= src->minform;
	ptr->name = azstrstab (src->name, stab);
	ptr->classID	= src->classID;
	ptr->OOB	= src->OOBid;
	ptr->OOBrid	= src->OOBnum;
	ptr->size	= src->size;
	ptr->cost	= src->cost;
	ptr->survive	= src->survive;
	ptr->leader	= src->leader;
	ptr->exp	= src->exp;
	ptr->mor	= src->morale;
	ptr->sup	= src->supp;
	ptr->status	= src->status;
	ptr->entr	= src->entr;
	ptr->smkdev	= src->smoke_dev;
	ptr->smkammo	= src->smoke_ammo;
	ptr->crew	= src->crew;
	ptr->range	= src->range;
	ptr->stance_x	= src->stance_x;
	ptr->stance_y	= src->stance_y;
	ptr->loader	= src->loader;
	ptr->load_cap	= src->load_cap;
	ptr->load_cost	= src->load_cost;
	ptr->radio	= src->radio;
	ptr->rof	= src->rof;
	ptr->tgt	= src->targetting;
	ptr->rf		= src->rangefinder;
	ptr->fc		= src->firec;
	ptr->iv		= src->irvision;
	ptr->swim	= src->swim;
	ptr->men	= src->men;
	ptr->men_ori	= src->ori_men;
	ptr->speed	= src->speed;
	ptr->moves	= src->moves;
	ptr->damage	= src->damage;
	ptr->movdir	= src->rotation1;
	ptr->shtdir	= src->rotation2;
	ptr->target	= src->target;
	ptr->UTGidx	= src->UTGidx;
	ptr->SPECIAL_OU	= src->SPECIAL_OU;
	ptr->SPECIAL[0]	= src->SPECIAL1;
	ptr->SPECIAL[1]	= src->SPECIAL2;
	ptr->SPECIAL[2]	= src->SPECIAL3;
	ptr->SPECIAL[3]	= src->SPECIAL4;
	ptr->SPECIAL[4]	= src->SPECIAL5;

	if (cfg.withUD) {
		SPWAW_UD *UD = &(ptr->UD);
		UD_init (UD, sizeof (*src));
		UD_ADD (UD, src, __data000);
		UD_ADD (UD, src, __data001);
		UD_ADD (UD, src, __data01);
		UD_ADD (UD, src, __data02);
		UD_ADD (UD, src, __data03);
		UD_ADD (UD, src, __data04);
		UD_ADD (UD, src, __data05);
		UD_ADD (UD, src, __data07);
		UD_ADD (UD, src, __data08);
		UD_ADD (UD, src, __data09);
		UD_ADD (UD, src, __data10);
		UD_ADD (UD, src, __data11);
		UD_ADD (UD, src, __data12);
		UD_ADD (UD, src, __data13);
		UD_ADD (UD, src, __data140);
		UD_ADD (UD, src, __data141);
		UD_ADD (UD, src, __data15);
		UD_ADD (UD, src, __data160);
		UD_ADD (UD, src, __data161);
		UD_ADD (UD, src, __data170);
		UD_ADD (UD, src, __data171);
		UD_ADD (UD, src, __data18);
		UD_ADD (UD, src, __data19);
		UD_ADD (UD, src, __data21);
		UD_ADD (UD, src, __data22);
		UD_ADD (UD, src, __data23);
		UD_ADD (UD, src, __data24);
		UD_ADD (UD, src, __data25);
		UD_ADD (UD, src, __data26);
	}

	(*idx)++;
	return (SPWERR_OK);
}

SPWAW_ERROR
sec01_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, UNIT_LIST &ul1, UNIT_LIST &ul2)
{
	SPWAW_ERROR	rc;
	UNIT		*data;
	USHORT		i, idx;

	CNULLARG (src); CNULLARG (dst);

	data = src->sec01.u.d.units;

	// First try the new method of counting units
	unitcount (data, UNITP1POSSTART, UNITP1POSEND, 1, &(dst->raw.OOBp1.formations), ul1);
	unitcount (data, UNITP2POSSTART, UNITP2POSEND, 2, &(dst->raw.OOBp2.formations), ul2);
	if (ul2.cnt == 0) {
		memset (&ul2, 0, sizeof (ul2));
		unitcount (data, UNITP1POSSTART, UNITP2POSEND, 2, &(dst->raw.OOBp2.formations), ul2);
	}

	// If the new method failed, try the old method
	if ((ul1.cnt == 0) || (ul2.cnt == 0)) {
		memset (&ul1, 0, sizeof (ul1));
		memset (&ul2, 0, sizeof (ul2));
		unitcount_oldmethod (data, &(dst->raw.OOBp1.formations), ul1, &(dst->raw.OOBp2.formations), ul2);
	}

	rc = setup (&(dst->raw.OOBp1.units), ul1); ROE ("setup(struct01/OOBp1)");
	for (idx = i = 0; i<ul1.cnt; i++) {
		rc = add_unit (&(data[ul1.list[i]]), ul1.list[i], dst->raw.OOBp1.units.raw, &idx, &(dst->raw.OOBp1.formations), stab);
		ROE ("add_unit(OOBp1)");
	}
	rc = build_uridx (&(dst->raw.OOBp1.units)); ROE ("build_uridx(OOBp1)");

	rc = setup (&(dst->raw.OOBp2.units), ul2); ROE ("setup(struct01/OOBp2)");
	for (idx = i = 0; i<ul2.cnt; i++) {
		rc = add_unit (&(data[ul2.list[i]]), ul2.list[i], dst->raw.OOBp2.units.raw, &idx, &(dst->raw.OOBp2.formations), stab);
		ROE ("add_unit(OOBp2)");
	}
	rc = build_uridx (&(dst->raw.OOBp2.units)); ROE ("build_uridx(OOBp2)");

	return (SPWERR_OK);
}
