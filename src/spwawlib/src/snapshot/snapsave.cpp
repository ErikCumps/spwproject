/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "snapshot/snapfile.h"
#include "snapshot/translate.h"
#include "strtab/strtab.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

#define	setB(name)	dst->b.##name = src->raw.game.battle.##name
#define	setC(name)	dst->c.##name = src->raw.game.campaign.##name

static void
prep_snap (SPWAW_SNAPSHOT *src, SNAP *dst, STRTAB *stab)
{
	int	i;

	dst->cmt.title = STRTAB_getidx (stab, src->raw.game.cmt.title);
	dst->cmt.mapsrc = STRTAB_getidx (stab, src->raw.game.cmt.mapsrc);

	setB (year); setB (month); setB (day); setB (hour);
	dst->b.location = STRTAB_getidx (stab, src->raw.game.battle.location);
	setB (terrain); setB (weather); setB (visibility); setB (turn); setB (turn_max);
	setB (OOB_p1); setB (OOB_p2); setB (OOB_p3); setB (OOB_p4);
	setB (miss_p1); setB (miss_p2); setB (credit);

	for (i=0; i<SNAP_VHEXCNT; i++) {
		dst->b.vhex[i].x = src->raw.game.battle.vhex[i].x;
		dst->b.vhex[i].y = src->raw.game.battle.vhex[i].y;
		dst->b.vhex[i].value = src->raw.game.battle.vhex[i].value;
		dst->b.vhex[i].owner = src->raw.game.battle.vhex[i].owner;
	}

	setC (campaign); setC (start_year); setC (start_month); setC (end_year); setC (end_month);
	setC (battles); setC (majvics); setC (minvics); setC (battles_max);
	setC (P1BLmen); setC (P1BLart); setC (P1BLsoft); setC (P1BLapc); setC (P1BLafv); setC (P1BLgliders); setC (P1BLair);
	setC (P2BLmen); setC (P2BLart); setC (P2BLsoft); setC (P2BLapc); setC (P2BLafv); setC (P2BLgliders); setC (P2BLair);
	setC (P1TLmen); setC (P1TLart); setC (P1TLsoft); setC (P1TLapc); setC (P1TLafv); setC (P1TLgliders); setC (P1TLair);
	setC (P2TLmen); setC (P2TLart); setC (P2TLsoft); setC (P2TLapc); setC (P2TLafv); setC (P2TLgliders); setC (P2TLair);
	setC (busy); setC (P1score); setC (P2score); setC (P1result); setC (P2result);
}

static SPWAW_ERROR
save_map (int fd, SPWAW_SNAP_MAP_RAW *map, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_MAPHDR	maphdr;
	ULONG		p0, p1;
	CBIO		cbio;

	memset (&maphdr, 0, sizeof (maphdr));

	maphdr.width  = map->width;
	maphdr.height = map->height;

	p0 = bseekget (fd);
	bseekmove (fd, sizeof (maphdr));

	maphdr.data = bseekget (fd) - p0;
	maphdr.size = map->size;

	cbio.data = (char *)(map->data); cbio.size = maphdr.size; cbio.comp = &(maphdr.comp);
	if (!cbwrite (fd, cbio, "raw map data", compress))
		FAILGOTO (SPWERR_FWFAILED, "cbwrite(raw map data) failed", handle_error);

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&maphdr, sizeof (maphdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(maphdr) failed", handle_error);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	bseekset (fd, p0);
	return (rc);
}

#define	setOF(name)	dst->##name = src->##name
#define	setOU(name)	dst->##name = src->##name
#define	setOL(name)	dst->##name = src->##name
#define	setOP(name)	dst->##name = src->##name

static void
prep_oobf (SPWAW_SNAP_OOB_FELRAW *src, SNAP_OOB_FEL *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SNAP_OOB_FEL));

	setOF (RID); setOF (FID);
	dst->name = STRTAB_getidx (stab, src->name);
	setOF (leader); setOF (hcmd); setOF (OOBrid); setOF (status);
}

static SPWAW_ERROR
save_oob_formations (int fd, ULONG pos, SNAP_OOBHDR &oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SBW		*sbw = NULL;
	ULONG		i;
	SNAP_OOB_FEL	f;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	oobhdr.fpos = bseekget (fd) - pos;
	oobhdr.fcnt = oob->formations.cnt;
	oobhdr.fstart = oob->formations.start;
	for (i=0; i<oobhdr.fcnt; i++) {
		prep_oobf (&(oob->formations.raw[i]), &f, stab);
		if (sbwrite (sbw, (char *)&f, sizeof (f)) != sizeof (f))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(formation data)", handle_error);
	}

	sbwrite_stop (sbw, &data, &size); sbw = NULL;
	oobhdr.fsize = size;

	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.fcomp);
	if (!cbwrite (fd, cbio, "formation data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(formation data)", handle_error);
	safe_free (data);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	return (rc);
}

static void
prep_oobu (SPWAW_SNAP_OOB_UELRAW *src, SNAP_OOB_UEL *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SNAP_OOB_UEL));

	dst->dutype = unittype2raw (src->dutype);
	setOU (RID); setOU (FRID); setOU (FMID); setOU (FSID);
	dst->name = STRTAB_getidx (stab, src->name);
	setOU (classID); setOU (OOB); setOU (OOBrid);
	setOU (size); setOU (cost); setOU (survive); setOU (leader);
	setOU (exp); setOU (mor); setOU (sup); setOU (status); setOU (entr);
	dst->aband = aband2raw (src->aband);
	setOU (smkdev); setOU (smkammo); setOU (crew);
	setOU (range); setOU (stance_x); setOU (stance_y);
	setOU (loader); setOU (load_cap); setOU (load_cost);
	setOU (contact); setOU (rof); setOU (tgt); setOU (rf); setOU (fc); setOU (iv);
	setOU (swim); setOU (men); setOU (men_ori); setOU (speed); setOU (moves);
	setOU (damage); setOU (movdir); setOU (shtdir); setOU (target); setOU (UTGidx);
//	setOU (SPECIAL_OU); setOU (SPECIAL[0]); setOU (SPECIAL[1]);
//	setOU (SPECIAL[2]); setOU (SPECIAL[3]); setOU (SPECIAL[4]);
}

static SPWAW_ERROR
save_oob_units (int fd, ULONG pos, SNAP_OOBHDR &oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SBW		*sbw = NULL;
	ULONG		i;
	SNAP_OOB_UEL	u;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	oobhdr.upos = bseekget (fd) - pos;
	oobhdr.ucnt = oob->units.cnt;
	for (i=0; i<oobhdr.ucnt; i++) {
		prep_oobu (&(oob->units.raw[i]), &u, stab);
		if (sbwrite (sbw, (char *)&u, sizeof (u)) != sizeof (u))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(unit data)", handle_error);
	}

	sbwrite_stop (sbw, &data, &size); sbw = NULL;
	oobhdr.usize = size;

	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.ucomp);
	if (!cbwrite (fd, cbio, "unit data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(unit data)", handle_error);
	safe_free (data);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	return (rc);
}

static void
prep_oobl (SPWAW_SNAP_OOB_LELRAW *src, SNAP_OOB_LEL *dst, STRTAB *stab)
{
	memset (dst, 0, sizeof (SNAP_OOB_LEL));

	setOL (RID); setOL (URID);
	dst->name = STRTAB_getidx (stab, src->name);
	setOL (rank); setOL (ral); setOL (inf); setOL (art); setOL (arm);
	setOL (kills); setOL (status);
}

static SPWAW_ERROR
save_oob_leaders (int fd, ULONG pos, SNAP_OOBHDR &oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SBW		*sbw = NULL;
	ULONG		i;
	SNAP_OOB_LEL	l;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	oobhdr.lpos = bseekget (fd) - pos;
	oobhdr.lcnt = oob->leaders.cnt;
	for (i=0; i<oobhdr.lcnt; i++) {
		prep_oobl (&(oob->leaders.raw[i]), &l, stab);
		if (sbwrite (sbw, (char *)&l, sizeof (l)) != sizeof (l))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(leader data)", handle_error);
	}

	sbwrite_stop (sbw, &data, &size); sbw = NULL;
	oobhdr.lsize = size;

	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.lcomp);
	if (!cbwrite (fd, cbio, "leader data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(leader data)", handle_error);
	safe_free (data);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	return (rc);
}

static void
prep_oobp (SPWAW_SNAP_OOB_PELRAW *src, SNAP_OOB_PEL *dst, STRTAB * /*stab*/)
{
	memset (dst, 0, sizeof (SNAP_OOB_PEL));

	setOP (RID); setOP (URID); setOP (x); setOP (y); setOP (seen);
}

static SPWAW_ERROR
save_oob_positions (int fd, ULONG pos, SNAP_OOBHDR &oobhdr, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SBW		*sbw = NULL;
	ULONG		i;
	SNAP_OOB_PEL	p;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	oobhdr.ppos = bseekget (fd) - pos;
	oobhdr.pcnt = oob->positions.cnt;
	for (i=0; i<oobhdr.pcnt; i++) {
		prep_oobp (&(oob->positions.raw[i]), &p, stab);
		if (sbwrite (sbw, (char *)&p, sizeof (p)) != sizeof (p))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(position data)", handle_error);
	}

	sbwrite_stop (sbw, &data, &size); sbw = NULL;
	oobhdr.psize = size;

	cbio.data = data; cbio.size = size; cbio.comp = &(oobhdr.pcomp);
	if (!cbwrite (fd, cbio, "position data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(position data)", handle_error);
	safe_free (data);

handle_error:
	if (data) safe_free (data);
	if (sbw) sbwrite_stop (sbw);
	return (rc);
}


static SPWAW_ERROR
save_oob (int fd, SPWAW_SNAP_OOB_RAW *oob, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	SNAP_OOBHDR	oobhdr;
	ULONG		p0, p1;

	memset (&oobhdr, 0, sizeof (oobhdr));

	p0 = bseekget (fd);
	bseekmove (fd, sizeof (oobhdr));

	rc = save_oob_formations (fd, p0, oobhdr, oob, stab, compress);
	ERRORGOTO ("save_oob_formations()", handle_error);

	rc = save_oob_units (fd, p0, oobhdr, oob, stab, compress);
	ERRORGOTO ("save_oob_units()", handle_error);

	rc = save_oob_leaders (fd, p0, oobhdr, oob, stab, compress);
	ERRORGOTO ("save_oob_leaders()", handle_error);

	rc = save_oob_positions (fd, p0, oobhdr, oob, stab, compress);
	ERRORGOTO ("save_oob_positions()", handle_error);

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&oobhdr, sizeof (oobhdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(oobhdr) failed", handle_error);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	bseekset (fd, p0);
	return (rc);
}

SPWAW_ERROR
snapsave (SPWAW_SNAPSHOT *src, int fd, bool do_oob, bool do_stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	SNAP_HEADER	mhdr;
	SNAP_SOURCE	shdr;
	SNAP_INFO	ihdr;
	STRTAB		*stab = NULL;
	SNAP		snap;
	CBIO		cbio;

	CNULLARG (src);
	stab = (STRTAB *)src->stab;

	p0 = bseekget (fd);

	memset (&mhdr, 0, sizeof (mhdr));
	memset (&shdr, 0, sizeof (shdr));
	memset (&ihdr, 0, sizeof (ihdr));
	memset (&snap, 0, sizeof (snap));

	memcpy (mhdr.magic, SNAP_MAGIC, strlen (SNAP_MAGIC));
	mhdr.version = SNAP_VERSION;

	bseekmove (fd, sizeof (mhdr));

	shdr.path = STRTAB_getidx (stab, src->src.path);
	shdr.file = STRTAB_getidx (stab, src->src.file);
	shdr.date = *((LONGLONG *)&(src->src.date));

	mhdr.src = bseekget (fd) - p0;
	if (!bwrite (fd, (char *)&shdr, sizeof (shdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(shdr) failed", handle_error);

	if (do_oob) {
		mhdr.oobdat = bseekget (fd) - p0;
		rc = SPWOOB_save (src->oobdat, fd, compress);
		ERRORGOTO ("SPWOOB_save()", handle_error);
	}

	ihdr.title	= STRTAB_getidx (stab, src->raw.game.cmt.title);
	ihdr.location	= STRTAB_getidx (stab, src->raw.game.battle.location);
	ihdr.turn	= src->raw.game.battle.turn;
	rc = SPWAW_date2stamp (&(src->game.battle.data.start), &(ihdr.start));
	ERRORGOTO ("SPWAW_date2stamp(start date)", handle_error);
	rc = SPWAW_date2stamp (&(src->game.battle.data.date), &(ihdr.date));
	ERRORGOTO ("SPWAW_date2stamp(turn date)", handle_error);
	ihdr.type       = src->type;
	ihdr.gametype	= src->gametype;

	mhdr.info = bseekget (fd) - p0;
	if (!bwrite (fd, (char *)&ihdr, sizeof (ihdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(ihdr) failed", handle_error);

	prep_snap (src, &snap, stab);

	mhdr.snap.data = bseekget (fd) - p0;
	mhdr.snap.size = sizeof (snap);

	cbio.data = (char *)&snap; cbio.size = mhdr.snap.size; cbio.comp = &(mhdr.snap.comp);
	if (!cbwrite (fd, cbio, "snapshot game data", compress))
		FAILGOTO (SPWERR_FWFAILED, "cbwrite(snapshot game data) failed", handle_error);

	mhdr.map = bseekget (fd) - p0;
	rc = save_map (fd, &(src->raw.game.map), compress);
	ERRORGOTO ("save_map(map)", handle_error);

	mhdr.oobp1 = bseekget (fd) - p0;
	rc = save_oob (fd, &(src->raw.OOBp1), stab, compress);
	ERRORGOTO ("save_oob(OOBp1)", handle_error);

	mhdr.oobp2 = bseekget (fd) - p0;
	rc = save_oob (fd, &(src->raw.OOBp2), stab, compress);
	ERRORGOTO ("save_oob(OOBp2)", handle_error);

	if (do_stab) {
		mhdr.stab = bseekget (fd) - p0;
		rc = STRTAB_fdsave (stab, fd, compress);
		ERRORGOTO ("STRTAB_fdsave()", handle_error);
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&mhdr, sizeof (mhdr))) FAILGOTO (SPWERR_FWFAILED, "bwrite(mhdr) failed", handle_error);
	bseekset (fd, p1);

	return (SPWERR_OK);

handle_error:
	bseekset (fd, p0);
	return (rc);
}
