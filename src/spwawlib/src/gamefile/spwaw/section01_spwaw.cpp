/** \file
 * The SPWaW Library - gamefile handling - SPWaW game data.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "gamefile/gamefile.h"
#include "gamefile/fulist.h"
#include "snapshot/index.h"
#include "snapshot/snapshot.h"
#include "utils/ud.h"
#include "utils/log.h"
#include "common/internal.h"

// The end-of-unitlist detection was not 100% correct.
//
// The problem was caused by leftover data from previous battles which are
// still present in the gamefile. This could lead to invalid unit counts and
// invalid unit data.
//
// What is known with some degree of certainty:
//	+ saved units for player #1 and player #2 can be mixed together
//	+ valid units and crews must have a valid name
//	+ valid units must reference a valid formation
//	+ the unit's OOB ID must match the formation leader unit's OOB ID
//	  (make sure to follow through to its unit if the leader is a crew!)
//	+ the number of units in a formation must match its OOB data
//	+ however, in some cases the OOB record ID for the formation is 0
//	+ there can be no units with duplicate formation/subformation IDs
//	+ crews may be saved before or after their associated units
//	+ crews must have a valid associated unit
//	+ units must have either no crew or a valid associated crew
//	+ units are saved in any order
//	+ units are not always saved in contiguous groups
//	+ units with a formation sub-ID >= 50 seem to be crews
//	+ units have been known to get their own, special, formation,
//	  for which no OOB record ID exists (it can not exist)
//
// What seem to be valid assumptions:
//	+ units with a formation sub-ID >= 60 seem to be special attached units
//	+ units with a formation sub-ID >= 110 seem to be crews of special attached units
//
// With the OOB record ID of the formation, units can be verified with their
// unit class ID - it should match the corresponding unit class ID recorded in
// the OOB formation data.
//
// The OOB record ID of a formation is recorded in the save game data, but in
// some case this value is 0 and the record ID must be detected in some other.
// way. This must be done by examining all the available OOB formation data.
//
// Therefore the following three-step unit detection process will be used:
//
// #1:	build a list of all candidate units which are not obviously wrong:
//	they must reference a valid formation, they may not belong to an already
//	processed formation and they may have no duplicate subformation IDs.
//
// #2:	try to find a valid OOB record ID for all formations:
//	+ use the save game value if available
//	+ via a direct formation name/date match if there is only one such match
//	+ via a unit class type match, assuming all associated units are valid
//	+ if the record ID is still found, assume it is a special formation:
//	  record the unit OOB record ID of the leader of the formation
//
// #3:	process all candidate units and use the formation OOB data to drop
//	any invalid candidates
//
// However, given all the above, we still seem to mis-identify units. This happens
// mostly with savegames from scenarios. Until now these cases are all false negatives,
// so the only bad thing about them is that we under-report. But it would be better
// if our unit detection would be 100% correct.

/* Convenience macro to build a simple comparable timestamp from a year/month date */
#define	SIMPLE_STAMP(yr_,mo_) ((yr_)*12+((mo_)-1))

/* Determines if a unit is a unit (or a crew/special attached unit) */
static inline bool
is_this_a_UNIT (UNIT *data, USHORT idx)
{
	return (data[idx].minform < CREWFSID);
}

/* Determines if a unit is a special attached unit (or a crew/regular unit) */
static inline bool
is_this_a_SPAU (UNIT *data, USHORT idx)
{
	return ((data[idx].minform >= SPAUFSID) && (data[idx].minform < SPAUCREWFSID));
}

/* Builds a list of all the candidate units */
static SPWAW_ERROR
find_candidate_units (UNIT *data, USHORT start, USHORT stop, BYTE player, FULIST &ful)
{
	USHORT	i;
	char	name[SPWAW_AZSNAME+1];
	UEL	*uel;
	FEL	*fel;

	UFDLOG1 ("find_candidate_units: player #%u\n", player);

	init_ULIST (ful.ul);

	for (i=start; i<=stop; i++)
	{
		// A unit (or crew) must have a valid name
		if (data[i].name[0] == '\0') continue;

		snprintf (name, sizeof (name) - 1, "%s", data[i].name); name[16] = '\0';

		uel = reserve_UEL (ful.ul);
		if (!uel) RWE (SPWERR_FAILED, "reserve_UEL() failed");

		// Record the basic unit info
		uel->d.RID    = i;
		memcpy (uel->d.name, data[i].name, SPWAW_AZSNAME);
		uel->d.FRID	= data[i].formID;
		uel->d.FSID	= data[i].minform;
		uel->d.OOB	= data[i].OOBid;
		uel->d.OOBrid	= data[i].OOBnum;
		uel->d.LRID	= data[i].leader;

		UFDLOG7 ("find_candidate_units: [%3.3u] F[%3.3u]<%3.3u> C<%5.5u> L<%5.5u> O<%3.3u> (%16.16s) ",
			uel->d.RID, uel->d.FRID, uel->d.FSID,
			data[i].crew, uel->d.LRID, data[i].OOBid,
			name);

		// The unit must reference a valid formation, so try to look it up in the FORMATION list
		fel = lookup_FLIST (ful.fl, uel->d.FRID);

		// Is the formation valid?
		if (!fel) {
			UFDLOG0 ("SKIPPED: invalid formation reference\n");
			continue;
		}

		// This unit references a valid formation.
		uel->d.FMID = fel->d.FID;

		// Record the unit's loader
		uel->d.loader = data[i].loader;

		// Is this a unit or a special attached unit?
		if (is_this_a_UNIT (data, i) || is_this_a_SPAU (data, i))
		{
			if (is_this_a_UNIT (data, i)) {
				// This is a candidate unit.
				uel->d.type = SPWAW_UNIT_TYPE_UNIT;
			} else {
				// This is a candidate special attached unit.
				uel->d.type = SPWAW_UNIT_TYPE_SPAU;
			}
			UFDLOG4 ("%4.4s #%u F<%3.3u,%3.3u> ", SPWAW_unittype2str(uel->d.type), player, uel->d.FMID, uel->d.FSID);

			// There can be no units with duplicate formation/subformation IDs
			if (fel->d.unit_lst[uel->d.FSID]) {
				UFDLOG0 ("SKIPPED: duplicate formation/subformation ID\n");
				continue;
			}

			UFDLOG0 ("CANDIDATE\n");

			// Keep track of abandoned units
			uel->d.aband = (uel->d.LRID == SPWAW_BADIDX) ? SPWAW_ASTAY : SPWAW_ANONE;

			// Keep track of its subformation ID for duplicate detection
			if (!add_unit_to_formation (fel, uel)) {
				RWE (SPWERR_BADSAVEDATA, "add_unit_to_formation() failed");
			}
		} else {
			// This is a candidate crew.
			uel->d.type = SPWAW_UNIT_TYPE_CREW;
			UFDLOG4 ("%4.4s #%u F<%3.3u,%3.3u> ", SPWAW_unittype2str(uel->d.type), player, uel->d.FMID, uel->d.FSID);

			UFDLOG0 ("CANDIDATE\n");

			// Keep track of abandoned units
			uel->d.aband = SPWAW_ALEFT;
		}
		if (!commit_UEL (ful.ul, uel)) {
			RWE (SPWERR_BADSAVEDATA, "commit_UEL() failed");
		}
	}
	UFDLOG1 ("find_candidate_units: %u candidates\n", ful.ul.cnt);

	return (SPWERR_OK);
}

/* Links up all candidate crews */
static SPWAW_ERROR
link_candidate_crews (FULIST &ful, UNIT *data)
{
	UEL	*p;
	UEL	*uel;

	// Link all CREWs
	p = ful.ul.head;
	while (p)
	{
		uel = p; p = p->l.next;
		if (uel->d.type != SPWAW_UNIT_TYPE_CREW) continue;

		UFDLOG5 ("link_candidate_crews: [%3.3u] CREW<%5.5u> F<%3.3u,%3.3u> (%16.16s) ",
			uel->d.RID, data[uel->d.RID].crew, uel->d.FMID, uel->d.FSID, uel->d.name);

		// A crew member must point to a valid unit
		USHORT unit = data[uel->d.RID].crew;
		UEL *parent = lookup_ULIST (ful.ul, unit);
		if (!parent) {
			UFDLOG0 ("DROPPED: invalid crew parent unit\n");
			goto drop_crew;
		}

		// The parent unit must have a crew record ID pointing back to the crew member
		USHORT crew = data[unit].crew;
		if (crew != uel->d.RID) {
			UFDLOG0 ("DROPPED: invalid crew parent unit linkage\n");
			goto drop_crew;
		}

		if (!add_crew_to_unit (uel, parent)) {
			RWE (SPWERR_BADSAVEDATA, "add_crew_to_unit() failed");
		}

		log ("LINKED\n");

		continue;

drop_crew:
		drop_UEL (ful.ul, uel);
		continue;
	}

	return (SPWERR_OK);
}

/* Tries to find the OOB record ID for the formation by looking for a single
 * match based on either type and date or type alone.
 */
static inline BYTE
search_oobrid_by_name (FEL *fel, SPWOOB *oob, SPWAW_DATE &date)
{
	SPWAW_TIMESTAMP	stamp;
	SPWOOB_DATA	*oobdata;
	BYTE		rv = 0;
	int		score=0, cnt = 0;
	SPWAW_DATE	s_date, e_date;
	SPWAW_TIMESTAMP	s_stmp, e_stmp;
	bool		mnad;

	UFDLOG4 ("  >>> SEARCH OOBRID - BY NAME: F<%3.3u>, T<%16.16s>, D<%4.4u/%2.2u>\n",
		fel->d.FID, fel->d.name, date.year, date.month);

	stamp = SIMPLE_STAMP (date.year, date.month);

	oobdata = spwoob_data (oob, fel->d.OOB);
	if (!oobdata) {
		// This shouldn't happen!
		log ("search_oobrid_by_name: INTERNAL ERROR: NO OOB DATA AVAILABLE! (fel->d.OOB = %u)\n", fel->d.OOB);
		return (0);
	}

	for (BYTE i=0; i<oobdata->fcnt; i++) {
		if (!oobdata->fdata[i].valid) continue;
		if (strncmp (fel->d.name, oobdata->fdata[i].name, SPWAW_AZSNAME) != 0) continue;

		SPWAW_set_date (s_date, oobdata->fdata[i].start_yr, oobdata->fdata[i].start_mo);
		s_stmp = SIMPLE_STAMP (s_date.year, s_date.month);

		SPWAW_set_date (e_date, oobdata->fdata[i].end_yr, 12);
		e_stmp = SIMPLE_STAMP (e_date.year, e_date.month);

		/* Only a single match on name+data, or a single match on name alone, is allowed */
		if ((s_stmp <= stamp) && (stamp <= e_stmp)) {
			mnad = true;
			if (score < 2) { score = 2; rv = i; cnt = 0; }
			cnt++;
		} else {
			mnad = false;
			if (score < 1) { score = 1; rv = i; cnt = 0; }
			if (score < 2) { cnt++; }
		}

		UFDTRACE6 ("    OOB #%3.3u <%16.16s> from=%4.4u/%2.2u to=%4.4u/%2.2u - ",
			i, oobdata->fdata[i].name,
			s_date.year, s_date.month,
			e_date.year, e_date.month);
		if (mnad) {
			UFDTRACE0 ("MATCH: name and date\n");
		} else {
			UFDTRACE0 ("MATCH: name only\n");
		}
	}

	if (cnt == 0) {
		UFDTRACE0 ("  NO MATCH FOUND!\n");
	} else if (cnt > 1) {
		UFDTRACE0 ("  TOO MANY MATCHES FOUND!\n");
	}

	UFDLOG1 ("  <<< rv=%u\n", rv);
	return (rv);
}

/* Tries to find the OOB record ID for the formation by performing an extensive
 * search on formation type, date, units name, units type and units date.
 */
static inline BYTE
search_oobrid_extensive (FEL *fel, SPWOOB *oob, SPWAW_DATE &date)
{
	SPWAW_TIMESTAMP	stamp;
	SPWOOB_DATA	*oobdata;
	BYTE		rv = 0;
	int		score = -100;
	int		s, ds, j, k;
	BYTE		cnt;
	USHORT		ids, uid, fid;
	SPWAW_DATE	s_date, e_date;
	SPWAW_TIMESTAMP	s_stmp, e_stmp;
	UEL		*uel;

	UFDLOG4 ("  >>> SEARCH OOBRID - EXTENSIVE: F<%3.3u>, T<%16.16s>, D<%4.4u/%2.2u>\n",
		fel->d.FID, fel->d.name, date.year, date.month);

	stamp = SIMPLE_STAMP (date.year, date.month);

	oobdata = spwoob_data (oob, fel->d.OOB);
	if (!oobdata) {
		// This shouldn't happen!
		log ("search_oobrid_extensive: INTERNAL ERROR: NO OOB DATA AVAILABLE! (fel->d.OOB = %u)\n", fel->d.OOB);
		return (0);
	}

	for (BYTE i=0; i<MAXFORMATIONUNITS; i++) {
		if ((uel = fel->d.unit_lst[i]) == NULL) continue;
		uel->d.OOBtype = oobdata->udata[uel->d.OOBrid].type;
		UFDTRACE6 ("  U<%3.3u> F<%3.3u,%3.3u> O<%3.3u> UT<%3.3u> <%16.16s>\n",
			uel->d.RID, uel->d.FMID, uel->d.FSID,
			uel->d.OOBrid, uel->d.OOBtype, uel->d.name);
	}

	for (BYTE i=0; i<oobdata->fcnt; i++) {
		if (!oobdata->fdata[i].valid) continue;
		if (strncmp (fel->d.name, oobdata->fdata[i].name, SPWAW_AZSNAME) != 0) continue;
		
		s = 0;

		/* Formation type name match is 1 point */
		ds = 1;

		SPWAW_set_date (s_date, oobdata->fdata[i].start_yr, oobdata->fdata[i].start_mo);
		s_stmp = SIMPLE_STAMP (s_date.year, s_date.month);
		SPWAW_set_date (e_date, oobdata->fdata[i].end_yr, 12);
		e_stmp = SIMPLE_STAMP (e_date.year, e_date.month);

		/* Formation availability period match is 2 extra points */
		if ((s_stmp <= stamp) && (stamp <= e_stmp))
		{
			ds += 2;
		}

		UFDTRACE7 ("  > OOB #%3.3u <%16.16s> from=%4.4u/%2.2u to=%4.4u/%2.2u - DS=%d\n",
			i, oobdata->fdata[i].name,
			s_date.year, s_date.month,
			e_date.year, e_date.month,
			ds);

		s += ds;

		/* Now try to match the units listed for this formation... */
		j = k = 0;
		while ((j < oobdata->fmecnt) && (k < MAXFORMATIONUNITS)) {
			cnt = oobdata->fdata[i].elements[j].cnt;
			ids = oobdata->fdata[i].elements[j].rid;
			if (!cnt) { j++; continue; }

			if (ids<oobdata->efstart) {
				uid = ids;

				SPWAW_set_date (s_date, oobdata->udata[uid].start_yr, oobdata->udata[uid].start_mo);
				s_stmp = SIMPLE_STAMP (s_date.year, s_date.month);
				SPWAW_set_date (e_date, oobdata->udata[uid].end_yr, 12);
				e_stmp = SIMPLE_STAMP (e_date.year, e_date.month);

				UFDTRACE5 ("  > + UNIT #%2.2u id=%4.4u cnt=%2.2u class=%3.3u <%16.16s>",
					j, uid, cnt,
					oobdata->udata[uid].type,
					oobdata->udata[uid].name);
				UFDTRACE4 (" from=%4.4u/%2.2u to=%4.4u/%2.2u",
					s_date.year, s_date.month,
					e_date.year, e_date.month);

				ds = 0;
				while (cnt--) {
					while (((uel = fel->d.unit_lst[k]) == NULL) && (k < MAXFORMATIONUNITS)) k++;
					if (k >= MAXFORMATIONUNITS) break;

					/* Unit name match is 1 extra point */
					if (strncmp (oobdata->udata[uid].name, uel->d.name, SPWAW_AZSNAME) == 0)
					{
						ds += 1;
					}

					/* Unit class match is 1 extra point */
					if (oobdata->udata[uid].type == uel->d.OOBtype)
					{
						ds += 1;
					}

					/* Unit availability period match is 1 extra point */
					if ((s_stmp <= stamp) && (stamp <= e_stmp))
					{
						ds += 1;
					}

					k++;
				}

				UFDTRACE2 (" - DS=%d, k=%u\n", ds, k);

				s += ds;
			} else {
				fid = ids-oobdata->efstart;

				SPWAW_set_date (s_date, oobdata->fdata[fid].start_yr, oobdata->fdata[fid].start_mo);
				s_stmp = SIMPLE_STAMP (s_date.year, s_date.month);
				SPWAW_set_date (e_date, oobdata->fdata[fid].end_yr, 12);
				e_stmp = SIMPLE_STAMP (e_date.year, e_date.month);

				ds = 0;

				/* Formation formation availability period match failure deducts 1 extra point */
				if ((stamp < s_stmp) || (e_stmp < stamp))
				{
					ds -= 1;
				}

				UFDTRACE4 ("  > + FORM #%2.2u id=%4.4u cnt=%2.2u <%16.16s>",
					i, fid, cnt, oobdata->fdata[fid].name);
				UFDTRACE4 (" from=%4.4u/%2.2u to=%4.4u/%2.2u",
					s_date.year, s_date.month,
					e_date.year, e_date.month);
				UFDTRACE1 (" - DS=%d\n",
					ds);

				s += ds;
			}
			j++;
		}
		UFDTRACE4 ("    SUGGESTED: #%3.3u <%16.16s>, score: %d, uc: %u\n",
			i, oobdata->fdata[i].name, s, k);

		if (s > score) {
			UFDTRACE2 ("  ++SELECTED: current best score %d (previous %d)\n",
				s, score);
			rv = i;
			score = s;
		}

	}

	UFDLOG1 ("  <<< rv=%u\n", rv);
	return (rv);

}

/* Determines the maximum number of units for the formation identified by the OOB record ID */
static inline bool
formation_unitcount (SPWOOB *OOB, USHORT OOBid, BYTE OOBrid, BYTE &cnt)
{
	SPWOOB_DATA	*oobdata;
	SPWOOB_FDATA	*data;
	unsigned int	max;

	/* If the formation OOB record ID is unknown we cannot make any guesses
	 * about the maximum number of units the formation may contain! */
	if (!OOBrid) return (false);

	oobdata = spwoob_data (OOB, (BYTE)(OOBid & 0xFF));
	if (!oobdata) return (false);

	data = &(oobdata->fdata[OOBrid]);

	max = 0;
	for (int i=0; i<oobdata->fmecnt; i++)
	{
		if ((data->elements[i].rid == 0) || (data->elements[i].rid >= oobdata->efstart)) continue;
		max += data->elements[i].cnt;
	}
	if (max > MAXFORMATIONUNITS) max = MAXFORMATIONUNITS;

	cnt = (BYTE)(max & 0xFF);
	return (true);
}

/* Determines the OOB record ID for all the formations in the formation list */
static SPWAW_ERROR
find_formation_oobrids (FULIST &ful, SPWOOB *OOB, SPWAW_DATE &date)
{
	FEL	*p;
	FEL	*fel;
	UEL	*ldru;

	p = ful.fl.head;
	while (p)
	{
		fel = p; p = p->l.next;

		UFDLOG5 ("find_formation_oobrids: FORMATION: P<%1.1u> ID<%3.3u> L<%5.5u> O<%3.3u> (%16.16s)\n",
			fel->d.player, fel->d.FID, fel->d.leader, fel->d.OOBrid, fel->d.name);

		if (!fel->d.unit_cnt) {
			UFDTRACE0 ("no units recorded for formation - dropping formation!\n");
			drop_FEL (ful.fl, fel);
			continue;
		}

		ldru = lookup_ULIST (ful.ul, fel->d.leader);
		if (ldru && (ldru->d.type == SPWAW_UNIT_TYPE_CREW)) {
			ldru = ldru->d.link.parent;
		}
		if (!ldru) {
			UFDTRACE0 ("no leader unit recorded for formation - dropping formation!\n");
			drop_FEL (ful.fl, fel);
			continue;
		}

		fel->d.OOB = ldru->d.OOB;

		if (fel->d.OOBrid == 0) {
			// If there is no recorded OOB record ID,
			// try a direct formation name/date match search:
			fel->d.OOBrid = search_oobrid_by_name (fel, OOB, date);
		}

		if (fel->d.OOBrid == 0) {
			// If there is still no OOB record ID,
			// try an extensive search:
			fel->d.OOBrid = search_oobrid_extensive (fel, OOB, date);
		}

		formation_unitcount (OOB, fel->d.OOB, fel->d.OOBrid, fel->d.unit_cnt);

		fel = fel->l.next;
	}

	return (SPWERR_OK);
}

/* Verifies all the candidate units and drops invalid units */
static SPWAW_ERROR
verify_candidate_units (FULIST &ful)
{
	UEL	*p;
	UEL	*uel;
	FEL	*fel;
	USHORT	max_urid = 0;

	// Do a first quick initial verification pass over all UNITs to determine the last valid unit.
	// This will allow us later to wildcard some units that would otherwise be dropped.
	p = ful.ul.head;
	while (p)
	{
		uel = p; p = p->l.next;
		// Skip CREWs and SPAUs
		if (uel->d.type != SPWAW_UNIT_TYPE_UNIT) continue;
		// Skip units without a formation reference
		fel = uel->d.formation; if (!fel) continue;
		// Skip units with an invalid OOB ID
		if (uel->d.OOB != fel->d.OOB) continue;
		// Skip units that don't seem to belong to the formation (unless they're the leader)
		if ((uel->d.RID != fel->d.leader) && (fel->d.unit_cnt && (uel->d.FSID >= fel->d.unit_cnt))) continue;

		max_urid = uel->d.RID;
	}

	// First verify all UNITs and SPAUs
	p = ful.ul.head;
	while (p)
	{
		uel = p; p = p->l.next;
		if (	(uel->d.type != SPWAW_UNIT_TYPE_UNIT) &&
			(uel->d.type != SPWAW_UNIT_TYPE_SPAU)	)
		{
			continue;
		}

		UFDLOG5 ("verify_candidate_units: [%3.3u] %4.4s: F<%3.3u,%3.3u> (%16.16s) ",
			uel->d.RID,  SPWAW_unittype2str(uel->d.type), uel->d.FMID, uel->d.FSID, uel->d.name);

		// Drop all units without a formation reference
		fel = uel->d.formation;
		if (!fel) {
			UFDLOG0 ("DROPPED: invalid formation reference\n");
			goto drop_unit;
		}

		// Drop all units with an invalid OOB ID
		if (uel->d.OOB != fel->d.OOB) {
			UFDLOG0 ("DROPPED: invalid OOB ID\n");
			goto drop_unit;
		}

		// Never drop units that are leaders of their formation
		if (uel->d.RID == fel->d.leader) {
			UFDLOG0 ("ACCEPTED - LEADER\n");
			goto accept_unit;
		}

		// Units that don't seem to belong to the formation (according to the OOB info) should be dropped.
		// But units can be given a wildcard to stay if:
		// * the unit is not beyond the last valid unit
		// * the unit is an SPAU
		// * the unit is a verified loader (if it has loaded an accepted unit or crew)
		if (fel->d.unit_cnt && (uel->d.FSID >= fel->d.unit_cnt)) {
			if (uel->d.RID <= max_urid) {
				UFDLOG0 ("ACCEPTED - WILDCARD\n");
				goto accept_unit;
			} else if (uel->d.type == SPWAW_UNIT_TYPE_SPAU) {
				UFDLOG0 ("ACCEPTED - SPAU WILDCARD\n");
				goto accept_unit;
			} else {
				UFDLOG0 ("POSTPONED: invalid subformation ID - pending verified loader acceptance test\n");
				goto postpone_unit;
			}
		}

		UFDLOG0 ("ACCEPTED\n");

accept_unit:
		// Mark the indicated loader unit as verified
		if (uel->d.loader != SPWAW_BADIDX) {
			UEL *l = lookup_ULIST (ful.ul, uel->d.loader);
			if (l) l->d.vrfloader = true;
		}

		// Fix up the unit's leader record ID
		if (uel->d.LRID == SPWAW_BADIDX) {
			if (uel->d.link.crew) uel->d.LRID = uel->d.link.crew->d.LRID;
		}
		if (uel->d.LRID == SPWAW_BADIDX) {
			uel->d.LRID = uel->d.RID;
		}
		continue;

drop_unit:
		drop_UEL (ful.ul, uel);
		continue;

postpone_unit:
		uel->d.needvrfldrtst = true;
		continue;
	}

	// Verify all CREWs
	p = ful.ul.head;
	while (p)
	{
		uel = p; p = p->l.next;
		if (uel->d.type != SPWAW_UNIT_TYPE_CREW) continue;

		UFDLOG4 ("verify_candidate_units: [%3.3u] CREW: F<%3.3u,%3.3u> (%16.16s) ",
			uel->d.RID, uel->d.FMID, uel->d.FSID, uel->d.name);

		// Drop all crews without a parent unit
		if (!uel->d.link.parent) {
			UFDLOG0 ("DROPPED: invalid unit reference\n");
			goto drop_crew;
		}

		log ("ACCEPTED\n");

		// Mark the indicated loader unit as verified
		if (uel->d.loader != SPWAW_BADIDX) {
			UEL *l = lookup_ULIST (ful.ul, uel->d.loader);
			if (l) l->d.vrfloader = true;
		}

		// Fix up the crew's leader record ID
		if (uel->d.LRID == SPWAW_BADIDX) {
			if (uel->d.link.parent) uel->d.LRID = uel->d.link.parent->d.LRID;
		}
		if (uel->d.LRID == SPWAW_BADIDX) {
			uel->d.LRID = uel->d.RID;
		}

		continue;

drop_crew:
		drop_UEL (ful.ul, uel);
		continue;
	}

	// Finally perform a verified loader acceptance test for those units that require it
	p = ful.ul.head;
	while (p)
	{
		uel = p; p = p->l.next;
		if (!uel->d.needvrfldrtst) continue;

		UFDLOG5 ("verify_candidate_units: [%3.3u] %s: F<%3.3u,%3.3u> (%16.16s) ",
			uel->d.RID, "VLAT", uel->d.FMID, uel->d.FSID, uel->d.name);

		if (uel->d.vrfloader) {
			UFDLOG0 ("ACCEPTED\n");
		} else {
			UFDLOG0 ("DROPPED: not a verified loader unit\n");
			drop_UEL (ful.ul, uel);
		}
	}

	return (SPWERR_OK);
}

/* Builds a list of all the valid units in the savegame data */
static SPWAW_ERROR
unitcount (UNIT *data, USHORT start, USHORT stop, BYTE player, FULIST &ful, SPWOOB *OOB, SPWAW_DATE &date)
{
	SPWAW_ERROR	rc;

	// Step 1.a: find all candidate units
	rc = find_candidate_units (data, start, stop, player, ful);
	ROE ("find_candidate_units()");

	// Step 1.b: link up all candidate crews
	rc = link_candidate_crews (ful, data);
	ROE ("verify_candidate_crews()");

	// Early exit if no units are found at all
	if (ful.ul.cnt == 0) return (SPWERR_OK);

	// Step 2: discover the OOB record IDs for all the formations
	rc = find_formation_oobrids (ful, OOB, date);
	ROE ("find_formation_oobrids()");

	// Step 3: verify the candidate units, using the formation OOB info
	rc = verify_candidate_units (ful);
	ROE ("verify_candidate_units()");

	// Report the results
	UFDLOG1 ("unitcount: ul.cnt=%u\n", ful.ul.cnt);
	dump_FULIST (ful);

	return (SPWERR_OK);
}

static SPWAW_ERROR
setup (SPWAW_SNAP_OOB_URAW *dst, ULIST &up)
{
	SPWAW_SNAP_OOB_UELRAW	*p;

	p = safe_nmalloc (SPWAW_SNAP_OOB_UELRAW, up.cnt); COOM (p, "SPWAW_SNAP_OOB_UELRAW list");
	dst->cnt   = up.cnt;
	dst->raw   = p;
	dst->start = up.cnt ? up.head->d.RID : 0;

	return (SPWERR_OK);
}

static SPWAW_ERROR
add_unit (UNIT *src, UEL *p, SPWAW_SNAP_OOB_UELRAW *dst, USHORT *idx, STRTAB *stab)
{
	SPWAW_SNAP_OOB_UELRAW	*ptr;

	ptr = &(dst[*idx]);

	log ("add_unit: idx=%u, RID=%u, type=%s\n", *idx, p->d.RID, SPWAW_unittype2str(p->d.type));

	ptr->RID	= p->d.RID;
	ptr->dutype	= p->d.type;
	ptr->FRID       = p->d.FRID;
	ptr->FMID       = p->d.FMID;
	ptr->FSID       = p->d.FSID;
	ptr->name = azstrstab (src->name, stab);
	ptr->classID	= src->classID;
	ptr->OOB        = p->d.OOB;
	ptr->OOBrid     = p->d.OOBrid;
	ptr->size	= src->size;
	ptr->cost	= src->cost;
	ptr->survive	= src->survive;
	ptr->leader	= p->d.LRID;
	ptr->exp	= src->exp;
	ptr->mor	= src->morale;
	ptr->sup	= src->supp;
	ptr->status	= src->status;
	ptr->entr	= src->entr;
	ptr->aband	= p->d.aband;
	ptr->smkdev	= src->smoke_dev;
	ptr->smkammo	= src->smoke_ammo;
	ptr->crew	= src->crew;
	ptr->range	= src->range;
	ptr->stance_x	= src->stance_x;
	ptr->stance_y	= src->stance_y;
	ptr->loader	= p->d.loader;
	ptr->load_cap	= src->load_cap;
	ptr->load_cost	= src->load_cost;
	ptr->contact	= src->contact;
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
section01_spwaw_detection (GAMEDATA *src, SPWAW_SNAPSHOT *dst, FULIST &ful1, FULIST &ful2)
{
	SPWAW_ERROR	rc;
	UNIT		*data;
	SPWAW_DATE	date;

	CNULLARG (src); CNULLARG (dst);

	data = GDSPWAW(src)->sec01.u.d.units;

	SPWAW_set_date (date, dst->raw.game.battle.year + SPWAW_STARTYEAR, dst->raw.game.battle.month);

	// Count the available units for player #1
	rc = unitcount (data, UNITP1POSSTART, UNITP2POSEND, 1, ful1, dst->oobdat, date);
	ROE ("unitcount(OOBp1)");

	// Count the available units for player #2
	rc = unitcount (data, UNITP1POSSTART, UNITP2POSEND, 2, ful2, dst->oobdat, date);
	ROE ("unitcount(OOBp2)");

	// Verify unit detection
	if ((ful1.ul.cnt == 0) || (ful2.ul.cnt == 0))
		RWE (SPWERR_BADSAVEDATA, "failed to detect units");

	return (SPWERR_OK);
}

SPWAW_ERROR
section01_spwaw_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab, FULIST &ful1, FULIST &ful2)
{
	SPWAW_ERROR	rc;
	UNIT		*data;
	SPWAW_DATE	date;
	USHORT		idx;
	UEL		*p;

	CNULLARG (src); CNULLARG (dst);

	data = GDSPWAW(src)->sec01.u.d.units;

	SPWAW_set_date (date, dst->raw.game.battle.year + SPWAW_STARTYEAR, dst->raw.game.battle.month);

	// Setup and load the unit data for player #1
	rc = setup (&(dst->raw.OOBp1.units), ful1.ul); ROE ("setup(OOBp1)");
	p = ful1.ul.head; idx = 0;
	while (p) {
		rc = add_unit (&(data[p->d.RID]), p, dst->raw.OOBp1.units.raw, &idx, stab);
		ROE ("add_unit(OOBp1)");
		p = p->l.next;
	}
	rc = build_uridx (&(dst->raw.OOBp1.units)); ROE ("build_uridx(OOBp1)");

	// Setup and load the unit data for player #2
	rc = setup (&(dst->raw.OOBp2.units), ful2.ul); ROE ("setup(OOBp2)");
	p = ful2.ul.head; idx = 0;
	while (p) {
		rc = add_unit (&(data[p->d.RID]), p, dst->raw.OOBp2.units.raw, &idx, stab);
		ROE ("add_unit(OOBp2)");
		p = p->l.next;
	}
	rc = build_uridx (&(dst->raw.OOBp2.units)); ROE ("build_uridx(OOBp2)");

	return (SPWERR_OK);
}
