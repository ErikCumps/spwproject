/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "snapshot/snapshot.h"
#include "snapshot/snapfile.h"
#include "snapshot/translate.h"
#include "snapshot/snapfile_v10.h"
#include "snapshot/snapfile_v11.h"
#include "snapshot/snapfile_v12.h"
#include "snapshot/index.h"
#include "strtab/strtab.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

#define	getB(name)	dst->raw.game.battle.##name = src->b.##name
#define	getC(name)	dst->raw.game.campaign.##name = src->c.##name
#define	getM(name)	dst->raw.game.map.##name = src->m.##name

static void
load_snap (SNAP *src, STRTAB *stab, SPWAW_SNAPSHOT *dst)
{
	int	i;

	dst->raw.game.meta.title = STRTAB_getstr (stab, src->meta.title);
	dst->raw.game.meta.mapsrc = STRTAB_getstr (stab, src->meta.mapsrc);

	getB (year); getB (month); getB (day); getB (hour);
	dst->raw.game.battle.location = STRTAB_getstr (stab, src->b.location);
	getB (terrain); getB (weather); getB (visibility); getB (turn); getB (turn_max);
	getB (OOB_p1); getB (OOB_p2); getB (OOB_p3); getB (OOB_p4);
	getB (miss_p1); getB (miss_p2); getB (credit);

	for (i=0; i<SNAP_VHEXCNT; i++) {
		dst->raw.game.battle.vhex[i].x = src->b.vhex[i].x;
		dst->raw.game.battle.vhex[i].y = src->b.vhex[i].y;
		dst->raw.game.battle.vhex[i].value = src->b.vhex[i].value;
		dst->raw.game.battle.vhex[i].owner = src->b.vhex[i].owner;
	}

	getC (campaign); getC (start_year); getC (start_month); getC (end_year); getC (end_month);
	getC (battles); getC (majvics); getC (minvics); getC (battles_max);
	getC (P1BLmen); getC (P1BLart); getC (P1BLsoft); getC (P1BLapc); getC (P1BLafv); getC (P1BLgliders); getC (P1BLair);
	getC (P2BLmen); getC (P2BLart); getC (P2BLsoft); getC (P2BLapc); getC (P2BLafv); getC (P2BLgliders); getC (P2BLair);
	getC (P1TLmen); getC (P1TLart); getC (P1TLsoft); getC (P1TLapc); getC (P1TLafv); getC (P1TLgliders); getC (P1TLair);
	getC (P2TLmen); getC (P2TLart); getC (P2TLsoft); getC (P2TLapc); getC (P2TLafv); getC (P2TLgliders); getC (P2TLair);
	getC (busy); getC (P1score); getC (P2score); getC (P1result); getC (P2result);
}

#define	getMD(name)	dst->##name = src->##name

static void
load_map_data (SNAP_MAPDATA *src, SPWAW_SNAP_MAP_DRAW *dst)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_MAP_DRAW));

	getMD (height);
	getMD (has_T1); getMD (has_T2); getMD (has_T3); getMD (has_T4);
	dst->tfs.raw = src->tfs;
	getMD (conn_road1); getMD (conn_road2); getMD (conn_rail); getMD (conn_tram);
}

static SPWAW_ERROR
load_map_list (SBR *sbr, ULONG cnt, SPWAW_SNAP_MAP_DRAW *map, ULONG version)
{
	ULONG		i;
	SNAP_MAPDATA	d;

	for (i=0; i<cnt; i++) {
		/* We are now backwards compatible with version 11 and older */
		if (version <= SNAP_VERSION_V11) {
			snapshot_load_v11_map_data (sbr, &d);
		} else {
			if (sbread (sbr, (char *)&d, sizeof (d)) != sizeof (d))
				RWE (SPWERR_FRFAILED, "sbread(raw map data) failed");
		}
		load_map_data (&d, &(map[i]));
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
load_map (int fd, SPWAW_SNAP_MAP_RAW *map, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	SNAP_MAPHDR	maphdr;
	ULONG		mapcnt;
	long		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	pos = bseekget (fd);

	memset (&maphdr, 0, sizeof (maphdr));
	if (!bread (fd, (char *)&maphdr, sizeof (maphdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(maphdr)", handle_error);

	mapcnt = maphdr.width * maphdr.height;

	size = maphdr.size;
	data = safe_smalloc (char, size);
	COOMGOTO (data, "SNAP_MAPDATA buffer", handle_error);

	cbio.data = data; cbio.size = size; cbio.comp = &(maphdr.comp);
	if (!cbread (fd, cbio, "raw map data")) FAILGOTO (SPWERR_FRFAILED, "cbread(raw map data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	map->width  = maphdr.width;
	map->height = maphdr.height;
	map->size = mapcnt * sizeof(SPWAW_SNAP_MAP_DRAW);
	map->data = safe_nmalloc (SPWAW_SNAP_MAP_DRAW, mapcnt);
	COOMGOTO (map->data, "SPWAW_SNAP_MAP_DRAW map", handle_error);

	rc = load_map_list (sbr, mapcnt, map->data, version);
	ERRORGOTO ("load_map_list()", handle_error);

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	if (map->data) safe_free (map->data);
	return (rc);
}

#define	getOF(name)	dst->##name = src->##name
#define	getOU(name)	dst->##name = src->##name
#define	getOL(name)	dst->##name = src->##name
#define	getOP(name)	dst->##name = src->##name

static void
load_oobf (SNAP_OOB_FEL *src, SPWAW_SNAP_OOB_FELRAW *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_OOB_FELRAW));

	getOF (RID); getOF (FID);
	dst->name = STRTAB_getstr (stab, src->name);
	getOF (leader); getOF (hcmd); getOF (OOBrid); getOF (status);
}

static SPWAW_ERROR
load_oobf_list (SBR *sbr, USHORT cnt, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	ULONG		i;
	SNAP_OOB_FEL	f;

	for (i=0; i<cnt; i++) {
		/* We are now backwards compatible with version 11 and older */
		if (version <= SNAP_VERSION_V11) {
			snapshot_load_v11_oob_fel (sbr, &f);
		} else {
			if (sbread (sbr, (char *)&f, sizeof (f)) != sizeof (f))
				RWE (SPWERR_FRFAILED, "sbread(formation data)");
		}
		load_oobf (&f, &(oob->formations.raw[i]), stab);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
load_oob_formations (int fd, long pos, SNAP_OOBHDR oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	oob->formations.cnt = oobhdr.fcnt;
	size = oobhdr.fsize;
	data = safe_smalloc (char, size);
	COOMGOTO (data, "SPWAW_SNAP_OOB_FEL data", handle_error);

	bseekset (fd, pos + oobhdr.fpos);
	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.fcomp);
	if (!cbread (fd, cbio, "formation data")) FAILGOTO (SPWERR_FRFAILED, "cbread(formation data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	oob->formations.raw = safe_nmalloc (SPWAW_SNAP_OOB_FELRAW, oobhdr.fcnt);
	COOMGOTO (oob->formations.raw, "SPWAW_SNAP_OOB_FELRAW list", handle_error);

	rc = load_oobf_list (sbr, oobhdr.fcnt, oob, stab, version);
	ERRORGOTO ("load_oobf_list()", handle_error);

	rc = build_fridx (&(oob->formations)); ROE ("build_fridx()");

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

static void
load_oobu (SNAP_OOB_UEL *src, SPWAW_SNAP_OOB_UELRAW *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_OOB_UELRAW));

	dst->dutype = raw2unittype (src->dutype);
	getOU (RID); getOU (FRID); getOU (FMID); getOU (FSID);
	dst->name = STRTAB_getstr (stab, src->name);
	getOU (classID); getOU (OOB); getOU (OOBrid);
	getOU (size); getOU (cost); getOU (survive); getOU (leader);
	getOU (exp); getOU (mor); getOU (sup); getOU (status); getOU (entr);
	dst->aband = raw2aband (src->aband);
	getOU (smkdev); getOU (smkammo); getOU (crew);
	getOU (range); getOU (stance_x); getOU (stance_y);
	getOU (loader); getOU (load_cap); getOU (load_cost);
	getOU (contact); getOU (rof); getOU (tgt); getOU (rf); getOU (fc); getOU (iv);
	getOU (swim); getOU (men); getOU (men_ori); getOU (speed); getOU (moves);
	getOU (damage); getOU (movdir); getOU (shtdir); getOU (target); getOU (UTGidx);
//	getOU (SPECIAL_OU); getOU (SPECIAL[0]); getOU (SPECIAL[1]);
//	getOU (SPECIAL[2]); getOU (SPECIAL[3]); getOU (SPECIAL[4]);
}

static SPWAW_ERROR
load_oobu_list (SBR *sbr, USHORT cnt, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	ULONG		i;
	SNAP_OOB_UEL	u;

	for (i=0; i<cnt; i++) {
		/* We are now backwards compatible with versions 11 and 10 */
		if (version == SNAP_VERSION_V10) {
			snapshot_load_v10_oob_uel (sbr, &u);
		} else if (version == SNAP_VERSION_V11) {
			snapshot_load_v11_oob_uel (sbr, &u);
		} else {
			if (sbread (sbr, (char *)&u, sizeof (u)) != sizeof (u))
				RWE (SPWERR_FRFAILED, "sbread(unit data)");
		}
		load_oobu (&u, &(oob->units.raw[i]), stab);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
load_oob_units (int fd, long pos, SNAP_OOBHDR oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	oob->units.cnt = oobhdr.ucnt;
	size = oobhdr.usize;
	data = safe_smalloc (char, size);
	COOMGOTO (data, "SPWAW_SNAP_OOB_UEL data", handle_error);

	bseekset (fd, pos + oobhdr.upos);
	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.ucomp);
	if (!cbread (fd, cbio, "unit data")) FAILGOTO (SPWERR_FRFAILED, "cbread(unit data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	oob->units.raw = safe_nmalloc (SPWAW_SNAP_OOB_UELRAW, oobhdr.ucnt);
	COOMGOTO (oob->units.raw, "SPWAW_SNAP_OOB_UELRAW list", handle_error);

	rc = load_oobu_list (sbr, oobhdr.ucnt, oob, stab, version);
	ERRORGOTO ("load_oobu_list()", handle_error);

	rc = build_uridx (&(oob->units)); ROE ("build_uridx()");

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	/* For version 10, we also need to perform the legacy detection of units and crews */
	if (version == SNAP_VERSION_V10) {
		rc = snapshot_legacy_ldrcrw_detect (oobhdr.ucnt, oob);
		ERRORGOTO ("snapshot_legacy_ldrcrw_detect()", handle_error);
	}

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

static void
load_oobl (SNAP_OOB_LEL *src, SPWAW_SNAP_OOB_LELRAW *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_OOB_LELRAW));

	getOL (RID); getOL (URID);
	dst->name = STRTAB_getstr (stab, src->name);
	getOL (rank); getOL (ral); getOL (inf); getOL (art); getOL (arm);
	getOL (kills); getOL (status);
}

static SPWAW_ERROR
load_oobl_list (SBR *sbr, USHORT cnt, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	ULONG		i;
	SNAP_OOB_LEL	l;

	for (i=0; i<cnt; i++) {
		/* We are now backwards compatible with version 11 and older */
		if (version <= SNAP_VERSION_V11) {
			snapshot_load_v11_oob_lel (sbr, &l);
		} else {
			if (sbread (sbr, (char *)&l, sizeof (l)) != sizeof (l))
				RWE (SPWERR_FRFAILED, "sbread(leader data)");
		}
		load_oobl (&l, &(oob->leaders.raw[i]), stab);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
load_oob_leaders (int fd, long pos, SNAP_OOBHDR oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	oob->leaders.cnt = oobhdr.lcnt;
	size = oobhdr.lsize;
	data = safe_smalloc (char, size);
	COOMGOTO (data, "SPWAW_SNAP_OOB_LEL data", handle_error);

	bseekset (fd, pos + oobhdr.lpos);
	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.lcomp);
	if (!cbread (fd, cbio, "leader data")) FAILGOTO (SPWERR_FRFAILED, "cbread(leader data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	oob->leaders.raw = safe_nmalloc (SPWAW_SNAP_OOB_LELRAW, oobhdr.lcnt);
	COOMGOTO (oob->leaders.raw, "SPWAW_SNAP_OOB_LELRAW list", handle_error);

	rc = load_oobl_list (sbr, oobhdr.lcnt, oob, stab, version);
	ERRORGOTO ("load_oobl_list()", handle_error);

	rc = build_lridx (&(oob->leaders)); ROE ("build_lridx()");

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

static void
load_oobp (SNAP_OOB_PEL *src, SPWAW_SNAP_OOB_PELRAW *dst, STRTAB * /*stab*/)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_OOB_PELRAW));

	getOP (RID); getOP (URID); getOP (x); getOP (y); getOP (seen);
}

static SPWAW_ERROR
load_oobp_list (SBR *sbr, USHORT cnt, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab)
{
	ULONG		i;
	SNAP_OOB_PEL	p;

	for (i=0; i<cnt; i++) {
		if (sbread (sbr, (char *)&p, sizeof (p)) != sizeof (p))
			RWE (SPWERR_FRFAILED, "sbread(position data)");
		load_oobp (&p, &(oob->positions.raw[i]), stab);
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
load_oob_positions (int fd, long pos, SNAP_OOBHDR oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	oob->positions.cnt = oobhdr.pcnt;
	size = oobhdr.psize;
	data = safe_smalloc (char, size); COOMGOTO (data, "SPWAW_SNAP_OOB_PELRAW list", handle_error);

	bseekset (fd, pos + oobhdr.ppos);
	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.pcomp);
	if (!cbread (fd, cbio, "position data")) FAILGOTO (SPWERR_FRFAILED, "cbread(position data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	oob->positions.raw = safe_nmalloc (SPWAW_SNAP_OOB_PELRAW, oobhdr.pcnt);
	COOMGOTO (oob->positions.raw, "SPWAW_SNAP_OOB_PELRAW list", handle_error);

	rc = load_oobp_list (sbr, oobhdr.pcnt, oob, stab);
	ERRORGOTO ("load_oobp_list()", handle_error);

	rc = build_pridx (&(oob->positions)); ROE ("build_pridx()");

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

static SPWAW_ERROR
load_oob (int fd, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	SNAP_OOBHDR	oobhdr;

	pos = bseekget (fd);

	memset (&oobhdr, 0, sizeof (oobhdr));
	/* We are now backwards compatible with version 11 and older */
	if (version <= SNAP_VERSION_V11) {
		rc = snapshot_load_v11_oob_header (fd, &oobhdr);
		ERRORGOTO ("snapshot_load_v11_oob_header(snapshot OOB data hdr)", handle_error);
	} else {
		if (!bread (fd, (char *)&oobhdr, sizeof (oobhdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snapshot OOB data hdr)", handle_error);
	}

	rc = load_oob_formations (fd, pos, oobhdr, oob, stab, version);
	ERRORGOTO ("load_oob_formations()", handle_error);

	rc = load_oob_units (fd, pos, oobhdr, oob, stab, version);
	ERRORGOTO ("load_oob_units()", handle_error);

	rc = load_oob_leaders (fd, pos, oobhdr, oob, stab, version);
	ERRORGOTO ("load_oob_leaders()", handle_error);

	rc = load_oob_positions (fd, pos, oobhdr, oob, stab);
	ERRORGOTO ("load_oob_positions()", handle_error);

	return (SPWERR_OK);

handle_error:
	if (oob->positions.raw) safe_free (oob->positions.raw);
	if (oob->leaders.raw) safe_free (oob->leaders.raw);
	if (oob->units.raw) safe_free (oob->units.raw);
	if (oob->formations.raw) safe_free (oob->formations.raw);
	return (rc);
}

SPWAW_ERROR
snaploadhdrs (int fd, SNAP_HEADER *mhdr, SNAP_SOURCE *shdr, SNAP_INFO *ihdr, SNAP_OOBHDR *ohdr)
{
	SPWAW_ERROR	rc;
	long		p0;

	CNULLARG (mhdr); CNULLARG (shdr); CNULLARG (ihdr);
	clear_ptr (mhdr);
	clear_ptr (shdr);
	clear_ptr (ihdr);

	if (ohdr) {
		clear_ptr (ohdr);
	}

	p0 = bseekget (fd);

	if (!bread (fd, (char *)mhdr, sizeof (*mhdr), false)) RWE (SPWERR_FRFAILED, "bread(mhdr) failed");

	if (memcmp (mhdr->magic, SNAP_MAGIC, SNAP_MGCLEN) != 0)
		RWE (SPWERR_BADSAVEDATA, "snapshot header check failed");

	/* We are now backwards compatible with versions 10 and newer */
	if ((mhdr->version != SNAP_VERSION) && (mhdr->version < SNAP_VERSION_V10))
		RWE (SPWERR_INCOMPATIBLE, "snapshot header version check failed");

	bseekset (fd, mhdr->src + p0);
	if (!bread (fd, (char *)shdr, sizeof (*shdr), false)) RWE (SPWERR_FRFAILED, "bread(shdr) failed");

	bseekset (fd, mhdr->info + p0);
	/* We are now backwards compatible with versions 11 and 10 */
	if (mhdr->version == SNAP_VERSION_V10) {
		rc = snapshot_load_v10_info_header (fd, ihdr);
		ROE ("snapshot_load_v10_info_header(snapshot info hdr)");
	} else if (mhdr->version == SNAP_VERSION_V11) {
		rc = snapshot_load_v11_info_header (fd, ihdr);
		ROE ("snapshot_load_v11_info_header(snapshot info hdr)");
	} else if (mhdr->version == SNAP_VERSION_V12) {
		rc = snapshot_load_v12_info_header (fd, ihdr);
		ROE ("snapshot_load_v12_info_header(snapshot info hdr)");
	} else {
		if (!bread (fd, (char *)ihdr, sizeof (*ihdr), false))
			RWE (SPWERR_FRFAILED, "bread(ihdr) failed");
	}

	if (ohdr) {
		bseekset (fd, mhdr->oobp1 + p0);
		if (!bread (fd, (char *)ohdr, sizeof (*ohdr), false)) RWE (SPWERR_FRFAILED, "bread(ohdr) failed");
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
snaploadinfo (int fd, SPWAW_SNAPSHOT_INFO *info)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_HEADER	mhdr;
	SNAP_SOURCE	shdr;
	SNAP_INFO	ihdr;
	SNAP_OOBHDR	ohdr;
	STRTAB		*stab = NULL;
	char		*date = NULL;

	CSPWINIT;
	CNULLARG (info);
	clear_ptr (info);

	rc = STRTAB_new (&stab);
	ERRORGOTO ("STRTAB_new()", handle_error);

	rc = snaploadhdrs (fd, &mhdr, &shdr, &ihdr, &ohdr);
	ERRORGOTO ("snaploadhdrs()", handle_error);

	bseekset (fd, mhdr.stab);
	rc = STRTAB_fdload (stab, fd);
	ERRORGOTO ("STRTAB_fdload()", handle_error);

	strcpy (info->title, STRTAB_getstr (stab, ihdr.title));
	strcpy (info->location, STRTAB_getstr (stab, ihdr.location));

	rc = SPWAW_stamp2date (&(ihdr.start), &(info->start));
	ERRORGOTO ("STRTAB_stamp2date(ihdr.start)", handle_error);

	info->tdate.turn = ihdr.turn;
	rc = SPWAW_stamp2date (&(ihdr.date), &(info->tdate.date));
	ERRORGOTO ("STRTAB_stamp2date(ihdr.date)", handle_error);

	rc = SPWAW_date2str (&(info->tdate.date), &date);
	ERRORGOTO ("SPWAW_date2str(info->tdate.date)", handle_error);

	snprintf (info->stamp, sizeof (info->stamp) - 1, "%s, turn %u", date, info->tdate.turn);
	free (date);

	snprintf (info->filename, sizeof (info->filename) - 1, "%s\\%s", STRTAB_getstr (stab, shdr.path), STRTAB_getstr (stab, shdr.file));
	info->filedate = *((FILETIME *)&(shdr.date));

	info->type = (SPWAW_BATTLE_TYPE)ihdr.type;
	info->gametype = (SPWAW_GAME_TYPE)ihdr.gametype;
	info->savetype = (SPWAW_SAVE_TYPE)ihdr.savetype;

	STRTAB_free (&stab);

	return (SPWERR_OK);

handle_error:
	if (stab) STRTAB_free (&stab);
	return (rc);
}

SPWAW_ERROR
snapload (int fd, SPWAW_SNAPSHOT *dst, STRTAB *stabptr)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = stabptr;
	long		pos;
	SNAP_HEADER	mhdr;
	SNAP_SOURCE	shdr;
	SNAP_INFO	ihdr;
	SNAP		snap;
	CBIO		cbio;

	CNULLARG (dst);
	if (!stabptr) stab = (STRTAB *)dst->stab;

	pos = bseekget (fd);

	memset (&snap, 0, sizeof (snap));

	rc = snaploadhdrs (fd, &mhdr, &shdr, &ihdr);
	ERRORGOTO ("snaploadhdrs()", handle_error);

	if (mhdr.oobdat) {
		rc = SPWOOB_new (&(dst->oobdat));
		ERRORGOTO ("SPWOOB_new()", handle_error);
		dst->freeoobdat = true;

		bseekset (fd, pos + mhdr.oobdat);
		rc = SPWOOB_load (dst->oobdat, fd);
		ERRORGOTO ("SPWOOB_load()", handle_error);
	}

	if (!stabptr) {
		bseekset (fd, pos + mhdr.stab);
		rc = STRTAB_fdload (stab, fd);
		ERRORGOTO ("STRTAB_fdload()", handle_error);
	}

	dst->src.path = STRTAB_getstr (stab, shdr.path);
	dst->src.file = STRTAB_getstr (stab, shdr.file);
	dst->src.date = *((FILETIME *)&(shdr.date));

	bseekset (fd, mhdr.snap.data + pos);
	/* We are now backwards compatible with version 11 and older */
	if (mhdr.version <= SNAP_VERSION_V11) {
		rc = snapshot_load_v11_snap (fd, &mhdr, &snap);
		ROE ("snapshot_load_v11_snap(snapshot game data)");
	} else {
		cbio.data = (char *)&snap; cbio.size = mhdr.snap.size; cbio.comp = &(mhdr.snap.comp);
		if (!cbread (fd, cbio, "snapshot game data"))
			FAILGOTO (SPWERR_FRFAILED, "cbread(snapshot game data) failed", handle_error);
	}
	load_snap (&snap, stab, dst);

	bseekset (fd, pos + mhdr.map);
	rc = load_map (fd, &(dst->raw.game.map), mhdr.version);
	ERRORGOTO ("load_map(map)", handle_error);

	bseekset (fd, pos + mhdr.oobp1);
	rc = load_oob (fd, &(dst->raw.OOBp1), stab, mhdr.version);
	ERRORGOTO ("load_oob(oobP1)", handle_error);

	bseekset (fd, pos + mhdr.oobp2);
	rc = load_oob (fd, &(dst->raw.OOBp2), stab, mhdr.version);
	ERRORGOTO ("load_oob(OOBp2)", handle_error);

	dst->type = (SPWAW_BATTLE_TYPE)ihdr.type;
	dst->gametype = (SPWAW_GAME_TYPE)ihdr.gametype;
	dst->savetype = (SPWAW_SAVE_TYPE)ihdr.savetype;

	return (SPWERR_OK);

handle_error:
	snapclean (dst, stabptr);
	return (rc);
}
