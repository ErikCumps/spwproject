/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "dossier/dossier.h"
#include "dossier/dossier_file.h"
#include "dossier/dossier_file_v10.h"
#include "dossier/dossier_file_v11.h"
#include "dossier/dossier_file_v12.h"
#include "dossier/dossier_file_v13.h"
#include "dossier/dossier_file_v14.h"
#include "dossier/dossier_file_v15.h"
#include "dossier/dossier_file_v16.h"
#include "spwoob/spwoob_list.h"
#include "snapshot/snapshot.h"
#include "gamefile/savegame_descriptor.h"
#include "strtab/strtab.h"
#include "uht/uht.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

static SPWAW_BTURN *
link_bturn (SPWAW_BTURN *p, SPWAW_BTURN *pp)
{
	p->prev = pp;
	p->next = NULL;

	if (p->prev) p->prev->next = p;

	return (p);
}

static SPWAW_ERROR
dossier_load_battle_turns (int fd, SPWAW_BATTLE *dst, USHORT cnt, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_THEADER	*hdrs;
	SPWAW_BTURN	*p = NULL;
	SPWAW_BTURN	*pp;
	USHORT		i;

	pos = bseekget (fd);

	hdrs = safe_nmalloc (DOS_THEADER, cnt); COOMGOTO (hdrs, "DOS_THEADER list", handle_error);

	dst->tlen = cnt;
	dst->tlist = safe_nmalloc (SPWAW_BTURN *, cnt); COOMGOTO (dst->tlist, "SPWAW_BTURN* list", handle_error);

	if (!bread (fd, (char *)hdrs, cnt * sizeof (DOS_THEADER), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(snap hdrs)", handle_error);

	pp = NULL;
	for (i=0, pp=NULL; i<dst->tlen; i++) {
		p = safe_malloc (SPWAW_BTURN); COOMGOTO (p, "SPWAW_BTURN", handle_error);

		p->battle = dst;

		p->tdate.turn = hdrs[i].turn;
		rc = SPWAW_stamp2date (&(hdrs[i].date), &(p->tdate.date));
		ERRORGOTO ("SPWAW_stamp2date(snap hdr date)", handle_error);

		rc = snapnew (&(p->snap), dst->oobdat, stab);
		ERRORGOTO ("snapnew()", handle_error);

		bseekset (fd, pos + hdrs[i].snap);
		rc = snapload (fd, p->snap, stab);
		ERRORGOTO ("snapload()", handle_error);

		rc = snapint (p->snap);
		ERRORGOTO ("snapint()", handle_error);

		rc = dossier_prep_bturn_info (p);
		ERRORGOTO ("dossier_prep_bturn_info()", handle_error);

		dst->tlist[dst->tcnt++] = pp = link_bturn (p, pp);
		p = NULL;

		rc = dossier_fix_battle_info (pp, dst->tlist[0]);
		ERRORGOTO ("dossier_fix_battle_info()", handle_error);
	}
	dst->tfirst = dst->tlist[0];
	dst->tlast  = dst->tlist[dst->tcnt-1];

handle_error:
	// dossier cleanup done by caller
	if (p) dossier_clean_turn (p);
	if (hdrs) safe_free (hdrs);
	return (rc);
}

static SPWAW_BATTLE *
link_battle (SPWAW_BATTLE *p, SPWAW_BATTLE *pp)
{
	p->prev = pp;
	p->next = NULL;

	if (p->prev) p->prev->next = p;

	return (p);
}

#define	getBMRD(name)	dst->##name = src->##name

static void
load_battle_map_raw_data (DOS_BMDATA_RAW *src, SPWAW_SNAP_MAP_DRAW *dst)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_MAP_DRAW));

	getBMRD (height);
	getBMRD (has_T1); getBMRD (has_T2); getBMRD (has_T3); getBMRD (has_T4);
	dst->tfs.raw = src->tfs;
	getBMRD (conn_road1); getBMRD (conn_road2); getBMRD (conn_rail); getBMRD (conn_tram);
}

static SPWAW_ERROR
load_battle_map_raw_list (SBR *sbr, ULONG cnt, SPWAW_SNAP_MAP_DRAW *map, ULONG /*version*/)
{
	ULONG		i;
	DOS_BMDATA_RAW	d;

	for (i=0; i<cnt; i++) {
		if (sbread (sbr, (char *)&d, sizeof (d)) != sizeof (d))
			RWE (SPWERR_FRFAILED, "sbread(battle map raw data) failed");
		load_battle_map_raw_data (&d, &(map[i]));
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_load_battle_map_raw_data (int fd, long pos, DOS_BHEADER &hdr, SPWAW_SNAP_MAP_RAW *dst, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	ULONG		mapcnt;
	ULONG		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	if ((hdr.map.width == 0) && (hdr.map.height == 0) && (hdr.map.raw.size == 0)) {
		/* There is no data to load */
		return (SPWERR_OK);
	}

	mapcnt = hdr.map.width * hdr.map.height;

	/* Verify data sizes match */
	size = mapcnt * sizeof (DOS_BMDATA_RAW);
	if (hdr.map.raw.size != size) {
		log ("raw map data size %lu != expected size %lu (width=%lu, height=%lu)\n",
			hdr.map.raw.size, size, hdr.map.width, hdr.map.height);
		FAILGOTO (SPWERR_CORRUPT, "battle map raw data size mismatch", handle_error);
	}

	/* Load data block */
	data = safe_smalloc (char, size);
	COOMGOTO (data, "DOS_BMDATA_RAW buffer", handle_error);

	bseekset (fd, pos + hdr.map.raw.data);
	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.map.raw.comp);
	if (!cbread (fd, cbio, "battle map raw data")) FAILGOTO (SPWERR_FRFAILED, "cbread(battle map raw data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	dst->reference	= true;
	dst->width	= hdr.map.width;
	dst->height	= hdr.map.height;
	dst->size	= mapcnt * sizeof(SPWAW_SNAP_MAP_DRAW);
	dst->data	= safe_nmalloc (SPWAW_SNAP_MAP_DRAW, mapcnt);
	COOMGOTO (dst->data, "SPWAW_SNAP_MAP_DRAW map", handle_error);

	rc = load_battle_map_raw_list (sbr, mapcnt, dst->data, version);
	ERRORGOTO ("load_battle_map_raw_list()", handle_error);

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);
	return (SPWERR_OK);

handle_error:
	if (dst->data) safe_free (dst->data);
	memset (dst, 0, sizeof (dst));
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

#define	getBMD(name)	dst->##name = src->##name
#define	getBMDB(name)	dst->##name = (src->##name != 0)

static void
load_battle_map_data (DOS_BMDATA_MAP *src, SPWAW_SNAP_MAP_DATA *dst)
{
	memset (dst, 0, sizeof (SPWAW_SNAP_MAP_DATA));

	getBMD (h);
	getBMDB (water); getBMDB (bridge); getBMDB (road);
	getBMD (conn_road1); getBMD (conn_road2); getBMD (conn_rail); getBMD (conn_tram);
}

static SPWAW_ERROR
load_battle_map_list (SBR *sbr, ULONG cnt, SPWAW_SNAP_MAP_DATA *map, ULONG /*version*/)
{
	ULONG		i;
	DOS_BMDATA_MAP	d;

	for (i=0; i<cnt; i++) {
		if (sbread (sbr, (char *)&d, sizeof (d)) != sizeof (d))
			RWE (SPWERR_FRFAILED, "sbread(battle map data) failed");
		load_battle_map_data (&d, &(map[i]));
	}

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_load_battle_map_data (int fd, long pos, DOS_BHEADER &hdr, SPWAW_SNAP_MAP *dst, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	ULONG		mapcnt;
	ULONG		size;
	char		*data = NULL;
	CBIO		cbio;
	SBR		*sbr = NULL;

	if ((hdr.map.width == 0) && (hdr.map.height == 0) && (hdr.map.map.size == 0)) {
		/* There is no data to load */
		return (SPWERR_OK);
	}

	mapcnt = hdr.map.width * hdr.map.height;

	/* Verify data sizes match */
	size = mapcnt * sizeof (DOS_BMDATA_MAP);
	if (hdr.map.map.size != size) {
		log ("map data size %lu != expected size %lu (width=%lu, height=%lu)\n",
			hdr.map.map.size, size, hdr.map.width, hdr.map.height);
		FAILGOTO (SPWERR_CORRUPT, "battle map data size mismatch", handle_error);
	}

	/* Load data block */
	data = safe_smalloc (char, size);
	COOMGOTO (data, "DOS_BMDATA_MAP buffer", handle_error);

	bseekset (fd, pos + hdr.map.map.data);
	cbio.data = data; cbio.size = size; cbio.comp = &(hdr.map.map.comp);
	if (!cbread (fd, cbio, "battle map data")) FAILGOTO (SPWERR_FRFAILED, "cbread(battle map data) failed", handle_error);

	sbr = sbread_init (data, size);
	if (!sbr) FAILGOTO (SPWERR_FRFAILED, "sbread_init() failed", handle_error);

	dst->reference	= true;
	dst->width	= hdr.map.width;
	dst->height	= hdr.map.height;
	dst->data	= safe_nmalloc (SPWAW_SNAP_MAP_DATA, mapcnt);
	COOMGOTO (dst->data, "SPWAW_SNAP_MAP_DATA map", handle_error);

	rc = load_battle_map_list (sbr, mapcnt, dst->data, version);
	ERRORGOTO ("load_battle_map_list()", handle_error);

	sbread_stop (sbr); sbr = NULL;
	safe_free (data);
	return (SPWERR_OK);

handle_error:
	if (sbr) sbread_stop (sbr);
	if (data) safe_free (data);
	return (rc);
}

static SPWAW_ERROR
dossier_load_battle_map (int fd, long pos, DOS_BHEADER &hdr, SPWAW_BATTLE *dst, ULONG version)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	memset (&(dst->map.raw), 0, sizeof (dst->map.raw));
	memset (&(dst->map.map), 0, sizeof (dst->map.map));

	if (version <= DOSS_VERSION_V15) {
		/* This is an old dossier, there is no battle map to load */

		/* We must create a reference from the battle map of the first turn */
		snap_mapref_create (dst->tfirst->snap, &(dst->map.raw), &(dst->map.map));
	} else {
		/* This is a modern dossier, the battle map can be loaded */

		rc = dossier_load_battle_map_raw_data (fd, pos, hdr, &(dst->map.raw), version);
		ERRORGOTO ("dossier_load_battle_map_raw_data()", handle_error);

		rc = dossier_load_battle_map_data (fd, pos, hdr, &(dst->map.map), version);
		ERRORGOTO ("dossier_load_battle_map_data()", handle_error);

		/* We must set a reference for the battle map of the first turn */
		snap_mapref_set (&(dst->map.raw), &(dst->map.map), dst->tfirst->snap);
	}

	/* Finally, we must set a reference for the battle map of the other turns */
	for (USHORT i=1; i<dst->tlen; i++) {
		snap_mapref_set (&(dst->map.raw), &(dst->map.map), dst->tlist[i]->snap);
	}

	return (SPWERR_OK);

handle_error:
	if (dst->map.raw.data) safe_free (dst->map.raw.data);
	memset (&(dst->map.raw), 0, sizeof (dst->map.raw));
	if (dst->map.map.data) safe_free (dst->map.map.data);
	memset (&(dst->map.map), 0, sizeof (dst->map.map));
	return (rc);
}

static SPWAW_ERROR
dossier_load_battles (int fd, SPWAW_DOSSIER *dst, USHORT cnt, STRTAB *stab, ULONG version, SPWAW_DOSSIER_LOAD_CB *load_cb)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_BHEADER	*hdrs;
	SPWAW_BATTLE	*p = NULL;
	SPWAW_BATTLE	*pp;
	USHORT		i;
	CBIO		cbio;

	if (!cnt) return (SPWERR_OK);

	pos = bseekget (fd);

	hdrs = safe_nmalloc (DOS_BHEADER, cnt); COOMGOTO (hdrs, "DOS_BHEADER list", handle_error);

	dst->blen = cnt;
	dst->blist = safe_nmalloc (SPWAW_BATTLE *, cnt); COOMGOTO (dst->blist, "SPWAW_BATTLE* list", handle_error);

	/* We are now backwards compatible with versions 10 through 15 */
	if (version == DOSS_VERSION_V10) {
		rc = dossier_load_v10_battle_headers (fd, hdrs, cnt);
		ERRORGOTO ("dossier_load_v10_battle_headers(battle hdrs)", handle_error);
	} else 	if (version == DOSS_VERSION_V11) {
		rc = dossier_load_v11_battle_headers (fd, hdrs, cnt);
		ERRORGOTO ("dossier_load_v11_battle_headers(battle hdrs)", handle_error);
	} else 	if (version == DOSS_VERSION_V12) {
		rc = dossier_load_v12_battle_headers (fd, hdrs, cnt);
		ERRORGOTO ("dossier_load_v12_battle_headers(battle hdrs)", handle_error);
	} else 	if (version <= DOSS_VERSION_V15) {
		rc = dossier_load_v15_battle_headers (fd, hdrs, cnt);
		ERRORGOTO ("dossier_load_v15_battle_headers(battle hdrs)", handle_error);
	} else 	if (version <= DOSS_VERSION_V16) {
		rc = dossier_load_v16_battle_headers (fd, hdrs, cnt);
		ERRORGOTO ("dossier_load_v16_battle_headers(battle hdrs)", handle_error);
	} else {
		if (!bread (fd, (char *)hdrs, cnt * sizeof (DOS_BHEADER), false))
			FAILGOTO (SPWERR_FRFAILED, "bread(battle hdrs)", handle_error);
	}

	log ("[%s] dst->blen=%u\n", __FUNCTION__, dst->blen);
	pp = NULL;
	for (i=0; i<dst->blen; i++) {
		p = safe_malloc (SPWAW_BATTLE); COOMGOTO (p, "SPWAW_BATTLE", handle_error);

		p->dossier = dst;

		rc = SPWAW_stamp2date (&(hdrs[i].date), &(p->bdate.date));
		ERRORGOTO ("SPWAW_stamp2date(battle hdr date)", handle_error);

		p->location_data = STRTAB_getstr (stab, hdrs[i].location_data);
		if (hdrs[i].location != BADSTRIDX) {
			p->location = STRTAB_getstr (stab, hdrs[i].location);
		} else {
			p->location = STRTAB_add (stab, p->location_data);
		}

		p->OOB_p1   = (BYTE)(hdrs[i].OOB_p1 & 0xFF);
		p->OOB_p2   = (BYTE)(hdrs[i].OOB_p2 & 0xFF);
		p->miss_p1  = STRTAB_getstr (stab, hdrs[i].miss_p1);
		p->miss_p2  = STRTAB_getstr (stab, hdrs[i].miss_p2);
		p->meeting  = hdrs[i].meeting != 0;

		rc = SPWOOB_LIST_takeref (dst->oobdata, hdrs[i].oobdat, &(p->oobdat));
		ERRORGOTO ("SPWOOB_LIST_idx2spwoob(battle oob data index)", handle_error);

		if (hdrs[i].name != BADSTRIDX) {
			p->name = STRTAB_getstr (stab, hdrs[i].name);
		} else {
			p->name = NULL;
		}

		bseekset (fd, pos + hdrs[i].tlist);
		rc = dossier_load_battle_turns (fd, p, hdrs[i].tcnt, stab);
		ERRORGOTO ("dossier_load_battle_turns()", handle_error);

		if (p->tcnt) p->snap = p->tlist[0]->snap;

		if (hdrs[i].btlidx != SPWAW_NOBTLIDX) {
			p->bdate.btlidx = hdrs[i].btlidx;
		} else {
			p->bdate.btlidx = p->tcnt ? p->tlist[0]->snap->game.btlidx : SPWAW_NOBTLIDX;
		}
		SPWAW_BDATE (p->bdate, date, true);
		log ("i=%02u, p->bdate=%s, p->tcnt=%u\n", i, date, p->tcnt);

		rc = dossier_set_battle_props (p);
		ERRORGOTO ("dossier_set_battle_props()", handle_error);

		if (version > DOSS_VERSION_V12) {
			/* This is a modern dossier, the RA can be loaded */
			p->racnt = hdrs[i].racnt;
			p->ra = safe_nmalloc (SPWAW_DOSSIER_BURA, p->racnt); COOMGOTO (p->ra, "RA list", handle_error);

			bseekset (fd, pos + hdrs[i].ra.data);

			cbio.data = (char *)(p->ra); cbio.size = hdrs[i].ra.size; cbio.comp = &(hdrs[i].ra.comp);
			if (!cbread (fd, cbio, "compressed battle unit ra"))
				FAILGOTO (SPWERR_FRFAILED, "cbread(compressed battle unit ra) failed", handle_error);
		}

		rc = dossier_update_battle_info (p);
		ERRORGOTO ("dossier_update_battle_info()", handle_error);

		if (version <= DOSS_VERSION_V12) {
			/* This is an old dossier, the RA must be rebuilt */
			dossier_update_battle_rainfo (pp, p);
		}

		rc = dossier_load_battle_map (fd, pos, hdrs[i], p, version);
		ERRORGOTO ("dossier_load_battle_map()", handle_error);

		dst->blist[dst->bcnt++] = pp = link_battle (p, pp);
		p = NULL;

		if (load_cb && load_cb->on_btlload) { load_cb->on_btlload (load_cb->context); }
	}
	dst->bfirst = dst->blist[0];
	dst->blast  = dst->blist[dst->bcnt-1];

	rc = dossier_update_campaign_props (dst);
	ERRORGOTO ("dossier_update_campaign_props()", handle_error);

	rc = dossier_update_dossier_stats (dst);
	ERRORGOTO ("dossier_update_dossier_stats()", handle_error);

handle_error:
	// dossier cleanup done by caller
	if (p) dossier_clean_battle (p);
	if (hdrs) safe_free (hdrs);
	return (rc);
}

static SPWAW_ERROR
dossier_check_magic_version (DOS_MV_HEADER &hdr)
{
	if (memcmp (hdr.magic, DOSS_MAGIC, DOSS_MGCLEN) != 0)
		RWE (SPWERR_BADSAVEDATA, "dossier header check failed");

	/* We are now backwards compatible with versions 10 and newer */
	if ((hdr.version != DOSS_VERSION) && (hdr.version < DOSS_VERSION_V10))
		RWE (SPWERR_INCOMPATIBLE, "dossier header version check failed");

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_check_header (DOS_HEADER &hdr)
{
	if (hdr.oobdata == 0)
		RWE (SPWERR_BADSAVEDATA, "dossier contains no OOB data");

	if (hdr.stab == 0)
		RWE (SPWERR_BADSAVEDATA, "dossier contains no STAB data");

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_load_campaign_props (DOS_CMPPROPS *props, SPWAW_DOSSIER_CMPPROPS *dst)
{
	CNULLARG (props); CNULLARG(dst);

	dst->OOB	= props->OOB;
	SPWAW_stamp2date (&(props->start), &(dst->start));
	SPWAW_stamp2date (&(props->end), &(dst->end));
	dst->maxbtlcnt	= props->maxbtlcnt;
	dst->ifcnt	= props->ifcnt;
	dst->iucnt	= props->iucnt;
	dst->cfcnt	= props->cfcnt;
	dst->cucnt	= props->cucnt;

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_load_tracking (DOS_TRACKING *tracking, SPWAW_DOSSIER_TRACKING *dst, STRTAB *stab)
{
	CNULLARG (tracking); CNULLARG(dst); CNULLARG (stab);

	savegame_descriptor_init (dst->sgd, tracking->gametype, tracking->savetype, tracking->oobdir, tracking->path, tracking->base, stab);
	dst->filename	= STRTAB_getstr (stab, tracking->filename);
	dst->filedate	= *((FILETIME *)&(tracking->filedate));

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_loadinfo (int fd, SPWAW_DOSSIER_INFO *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_MV_HEADER	mvhdr;
	DOS_HEADER	hdr;
	STRTAB		*stab = NULL;

	CNULLARG (dst);

	pos = bseekget (fd);

	memset (&mvhdr, 0, sizeof (mvhdr));

	if (!bread (fd, (char *)&mvhdr, sizeof (mvhdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(mvhdr)", handle_error);

	rc = dossier_check_magic_version (mvhdr);
	ERRORGOTO ("dossier_check_header()", handle_error);

	memset (&hdr, 0, sizeof (hdr));

	/* We are now backwards compatible with versions 10 through 14 */
	if (mvhdr.version == DOSS_VERSION_V10) {
		rc = dossier_load_v10_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v10_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V11) {
		rc = dossier_load_v11_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v11_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V12) {
		rc = dossier_load_v12_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v12_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V13) {
		rc = dossier_load_v13_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v13_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V14) {
		rc = dossier_load_v14_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v14_header(dossier hdr)", handle_error);
	} else {
		if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
			FAILGOTO (SPWERR_FRFAILED, "bread(hdr)", handle_error);
	}

	rc = dossier_check_header (hdr);
	ERRORGOTO ("dossier_check_header()", handle_error);

	rc = dossier_load_campaign_props (&(hdr.props), &(dst->props));
	ERRORGOTO ("dossier_load_campaign_props()", handle_error);

	rc = STRTAB_new (&stab);
	ERRORGOTO ("STRTAB_new()", handle_error);

	bseekset (fd, pos + hdr.stab);
	rc = STRTAB_fdload (stab, fd);
	ERRORGOTO ("STRTAB_fdload()", handle_error);

	dst->gametype = (SPWAW_GAME_TYPE)hdr.gametype;
	snprintf (dst->name, sizeof (dst->name) - 1, "%s", STRTAB_getstr (stab, hdr.name));
	snprintf (dst->comment, sizeof (dst->comment) - 1, "%s", STRTAB_getstr (stab, hdr.comment));
	dst->type = (SPWAW_DOSSIER_TYPE)hdr.type;

	dst->btlcnt = hdr.bcnt;

	if (dst->type == SPWAW_MEGACAM_DOSSIER) {
		rc = dossier_load_tracking (&(hdr.tracking), &(dst->tracking), stab);
		ERRORGOTO ("dossier_load_tracking()", handle_error);
	}

	STRTAB_free (&stab);

	return (SPWERR_OK);

handle_error:
	if (stab) STRTAB_free (&stab);
	return (rc);
}

SPWAW_ERROR
dossier_load (int fd, SPWAW_DOSSIER *dst, SPWAW_DOSSIER_LOAD_CB *load_cb)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		pos;
	DOS_MV_HEADER	mvhdr;
	DOS_HEADER	hdr;
	STRTAB		*stab = NULL;

	CNULLARG (dst);
	stab = (STRTAB *)dst->stab;

	pos = bseekget (fd);

	memset (&mvhdr, 0, sizeof (mvhdr));

	if (!bread (fd, (char *)&mvhdr, sizeof (mvhdr), false))
		FAILGOTO (SPWERR_FRFAILED, "bread(mvhdr)", handle_error);

	rc = dossier_check_magic_version (mvhdr);
	ERRORGOTO ("dossier_check_header()", handle_error);

	memset (&hdr, 0, sizeof (hdr));

	/* We are now backwards compatible with versions 10 through 14 */
	if (mvhdr.version == DOSS_VERSION_V10) {
		rc = dossier_load_v10_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v10_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V11) {
		rc = dossier_load_v11_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v11_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V12) {
		rc = dossier_load_v12_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v12_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V13) {
		rc = dossier_load_v13_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v13_header(dossier hdr)", handle_error);
	} else if (mvhdr.version == DOSS_VERSION_V14) {
		rc = dossier_load_v14_header (fd, &hdr);
		ERRORGOTO ("dossier_load_v14_header(dossier hdr)", handle_error);
	} else {
		if (!bread (fd, (char *)&hdr, sizeof (hdr), false))
			FAILGOTO (SPWERR_FRFAILED, "bread(hdr)", handle_error);
	}

	rc = dossier_check_header (hdr);
	ERRORGOTO ("dossier_check_header()", handle_error);

	rc = dossier_load_campaign_props (&(hdr.props), &(dst->props));
	ERRORGOTO ("dossier_load_campaign_props()", handle_error);

	bseekset (fd, pos + hdr.oobdata);
	/* We are now backwards compatible with version 10 */
	if (mvhdr.version == DOSS_VERSION_V10) {
		rc = dossier_load_v10_oobdata (fd, dst->oobdata);
		ERRORGOTO ("dossier_load_v10_oobdata()", handle_error);
	} else {
		rc = SPWOOB_LIST_load (dst->oobdata, fd);
		ERRORGOTO ("SPWOOB_LIST_load()", handle_error);
	}

	bseekset (fd, pos + hdr.stab);
	rc = STRTAB_fdload (stab, fd);
	ERRORGOTO ("STRTAB_fdload()", handle_error);

	dst->name = STRTAB_getstr (stab, hdr.name);
	dst->comment = STRTAB_getstr (stab, hdr.comment);
	if (hdr.savedir != BADSTRIDX) {
		dst->savedir = STRTAB_getstr (stab, hdr.savedir);
	} else {
		dst->savedir = NULL;
	}
	dst->oobdir = STRTAB_getstr (stab, hdr.oobdir);
	dst->type = (SPWAW_DOSSIER_TYPE)hdr.type;
	dst->gametype = (SPWAW_GAME_TYPE)hdr.gametype;

	if (load_cb && load_cb->on_started) { load_cb->on_started (load_cb->context, hdr.bcnt); }

	bseekset (fd, pos + hdr.blist);
	rc = dossier_load_battles (fd, dst, hdr.bcnt, stab, mvhdr.version, load_cb);
	ERRORGOTO ("dossier_load_battles()", handle_error);

	if (dst->type != SPWAW_STDALONE_DOSSIER) {
		log ("[%s] UHT handling\n", __FUNCTION__);

		/* We are now backwards compatible with version 12 and older */
		if (mvhdr.version <= DOSS_VERSION_V12) {
			/* This is an old dossier, the UHT must be rebuilt */
			rc = UHT_rebuild (&(dst->uht));
			ERRORGOTO ("UHT_rebuild()", handle_error);
		} else {
			/* This is a modern dossier, the UHT can be loaded */
			bseekset (fd, pos + hdr.uht);
			rc = UHT_load (fd, &(dst->uht));
			ERRORGOTO ("UHT_load()", handle_error);
		}
	}

	if (dst->type == SPWAW_MEGACAM_DOSSIER) {
		rc = dossier_load_tracking (&(hdr.tracking), &(dst->tracking), stab);
		ERRORGOTO ("dossier_load_tracking()", handle_error);
	}

	SPWOOB_LIST_debug_log (dst->oobdata, __FUNCTION__);

	if (load_cb && load_cb->on_finished) { load_cb->on_finished (load_cb->context); }

	return (SPWERR_OK);

handle_error:
	dossier_clean (dst);
	return (rc);
}
