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
#include "spwoob/spwoob_list.h"
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "uht/uht.h"
#include "fileio/fileio.h"
#include "common/internal.h"
#include "utils/compression.h"

static SPWAW_ERROR
dossier_save_battle_turns (SPWAW_BATTLE *src, int fd, USHORT *cnt, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	USHORT		i, idx;
	SPWAW_BTURN	*p = NULL;
	DOS_THEADER	*hdrs;

	CNULLARG (src); CNULLARG (cnt); CNULLARG (stab);
	*cnt = 0;

	p0 = bseekget (fd);

	hdrs = safe_nmalloc (DOS_THEADER, src->tcnt); COOMGOTO (hdrs, "DOS_THEADER list", handle_error);

	for (i=idx=0; i<src->tcnt; i++) {
		p = src->tlist[i]; if (!p) continue;

		rc = SPWAW_date2stamp (&(p->tdate.date), &(hdrs[idx].date));
		ERRORGOTO ("SPWAW_date2stamp(snap hdr date)", handle_error);

		hdrs[idx].turn = p->tdate.turn;

		bseekmove (fd, sizeof (DOS_THEADER));

		idx++;
	}

	*cnt = idx;

	for (i=idx=0; i<src->tcnt; i++) {
		p = src->tlist[i]; if (!p) continue;

		hdrs[idx].snap = bseekget (fd) - p0;
		snapsave (p->snap, fd, false, false, compress);

		idx++;
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)hdrs, (*cnt) * sizeof (DOS_THEADER)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(snap hdrs)", handle_error);
	bseekset (fd, p1);

	free (hdrs);

	return (SPWERR_OK);

handle_error:
	if (hdrs) free (hdrs);
	*cnt = 0;
	return (rc);
}

#define	setBMRD(name)	dst->##name = src->##name

static void
prep_battle_map_raw_data (SPWAW_SNAP_MAP_DRAW *src, DOS_BMDATA_RAW *dst)
{
	memset (dst, 0, sizeof (DOS_BMDATA_RAW));

	setBMRD (height);
	setBMRD (has_T1); setBMRD (has_T2); setBMRD (has_T3); setBMRD (has_T4);
	dst->tfs = src->tfs.raw;
	setBMRD (conn_road1); setBMRD (conn_road2); setBMRD (conn_rail); setBMRD (conn_tram);
}

static SPWAW_ERROR
dossier_save_battle_map_raw_data (SPWAW_SNAP_MAP_RAW *src, int fd, long hdrpos, DOS_BHEADER &hdr, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	ULONG		mapcnt;
	SBW		*sbw = NULL;
	DOS_BMDATA_RAW	d;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	hdr.map.raw.data = bseekget(fd) - hdrpos;
	hdr.map.raw.size = hdr.map.raw.comp = 0;

	if (!src->reference || ((src->width == 0) && (src->height == 0))) {
		/* There is no data to save */
		return (SPWERR_OK);
	}

	mapcnt = src->width * src->height;
	if (mapcnt == 0) {
		log ("map width=%lu, map height=%lu\n", src->width, src->height);
		FAILGOTO (SPWERR_FWFAILED, "invalid battle map dimensions", handle_error);
	}

	/* prepare data */
	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	for (ULONG i=0; i<mapcnt; i++) {
		prep_battle_map_raw_data (&(src->data[i]), &d);
		if (sbwrite (sbw, (char *)&d, sizeof (d)) != sizeof (d))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(battle map raw data)", handle_error);
	}

	sbwrite_stop (sbw, &data, &size); sbw = NULL;
	hdr.map.raw.size = size;

	/* save data block */
	cbio.data = data; cbio.size = hdr.map.raw.size; cbio.comp = &(hdr.map.raw.comp);
	if (!cbwrite (fd, cbio, "battle map raw data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(battle map raw data) failed", handle_error);
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbw) sbwrite_stop (sbw);
	if (data) safe_free (data);
	return (rc);
}

#define	setBMD(name)	dst->##name = src->##name

static void
prep_battle_map_data (SPWAW_SNAP_MAP_DATA *src, DOS_BMDATA_MAP *dst)
{
	memset (dst, 0, sizeof (DOS_BMDATA_RAW));

	setBMD (h);
	setBMD (water); setBMD (bridge); setBMD (road);
	setBMD (conn_road1); setBMD (conn_road2); setBMD (conn_rail); setBMD (conn_tram);
}

static SPWAW_ERROR
dossier_save_battle_map_data (SPWAW_SNAP_MAP *src, int fd, long hdrpos, DOS_BHEADER &hdr, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	ULONG		mapcnt;
	SBW		*sbw = NULL;
	DOS_BMDATA_MAP	d;
	char		*data = NULL;
	long		size;
	CBIO		cbio;

	hdr.map.map.data = bseekget(fd) - hdrpos;
	hdr.map.map.size = hdr.map.map.comp = 0;

	if (!src->reference || ((src->width == 0) && (src->height == 0))) {
		/* There is no data to save */
		return (SPWERR_OK);
	}

	mapcnt = src->width * src->height;
	if (mapcnt == 0) {
		log ("map width=%lu, map height=%lu\n", src->width, src->height);
		FAILGOTO (SPWERR_FWFAILED, "invalid battle map dimensions", handle_error);
	}

	/* prepare data */
	sbw = sbwrite_init (NULL, 0);
	if (!sbw) FAILGOTO (SPWERR_FWFAILED, "sbwrite_init() failed", handle_error);

	for (ULONG i=0; i<mapcnt; i++) {
		prep_battle_map_data (&(src->data[i]), &d);
		if (sbwrite (sbw, (char *)&d, sizeof (d)) != sizeof (d))
			FAILGOTO (SPWERR_FWFAILED, "sbwrite(battle map data)", handle_error);
	}

	sbwrite_stop (sbw, &data, &size); sbw = NULL;
	hdr.map.map.size = size;

	/* save data block */
	cbio.data = data; cbio.size = hdr.map.map.size; cbio.comp = &(hdr.map.map.comp);
	if (!cbwrite (fd, cbio, "battle map data", compress)) FAILGOTO (SPWERR_FWFAILED, "cbwrite(battle map data) failed", handle_error);
	safe_free (data);

	return (SPWERR_OK);

handle_error:
	if (sbw) sbwrite_stop (sbw);
	if (data) safe_free (data);
	return (rc);
}

static SPWAW_ERROR
dossier_save_battle_map (SPWAW_BATTLE *src, int fd, long hdrpos, DOS_BHEADER &hdr, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	rc = dossier_save_battle_map_raw_data (&(src->map.raw), fd, hdrpos, hdr, compress);
	ROE ("dossier_save_battle_map_raw_data()");

	rc = dossier_save_battle_map_data (&(src->map.map), fd, hdrpos, hdr, compress);
	ROE ("dossier_save_battle_map_data()");

	hdr.map.width  = src->map.map.width;
	hdr.map.height = src->map.map.height;

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_save_battles (SPWAW_DOSSIER *src, int fd, USHORT *cnt, STRTAB *stab, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	USHORT		i, idx;
	SPWAW_BATTLE	*p = NULL;
	DOS_BHEADER	*hdrs;
	CBIO		cbio;

	CNULLARG (src); CNULLARG (cnt); CNULLARG (stab);
	*cnt = 0;

	p0 = bseekget (fd);

	hdrs = safe_nmalloc (DOS_BHEADER, src->bcnt); COOMGOTO (hdrs, "DOS_BHEADER list", handle_error);

	for (i=idx=0; i<src->bcnt; i++) {
		p = src->blist[i]; if (!p) continue;

		rc = SPWAW_date2stamp (&(p->bdate.date), &(hdrs[idx].date));
		ERRORGOTO ("SPWAW_date2stamp(battle hdr date)", handle_error);

		hdrs[idx].location = STRTAB_getidx (stab, p->location);
		hdrs[idx].OOB_p1   = p->OOB_p1;
		hdrs[idx].OOB_p2   = p->OOB_p2;
		hdrs[idx].miss_p1  = STRTAB_getidx (stab, p->miss_p1);
		hdrs[idx].miss_p2  = STRTAB_getidx (stab, p->miss_p2);
		hdrs[idx].meeting  = p->meeting;
		hdrs[idx].btlidx   = p->bdate.btlidx;
		hdrs[idx].racnt    = p->racnt;

		rc = SPWOOB_LIST_spwoob2idx (src->oobdata, p->oobdat, &(hdrs[idx].oobdat));
		ERRORGOTO ("SPWOOB_LIST_spwoob2idx(battle oob data)", handle_error);

		if (p->name) {
			hdrs[idx].name = STRTAB_getidx (stab, p->name);
		} else {
			hdrs[idx].name = BADSTRIDX;
		}

		bseekmove (fd, sizeof (DOS_BHEADER));

		idx++;
	}

	*cnt = idx;

	for (i=idx=0; i<src->bcnt; i++) {
		p = src->blist[i]; if (!p) continue;

		hdrs[idx].tlist = bseekget (fd) - p0;
		rc = dossier_save_battle_turns (p, fd, &(hdrs[idx].tcnt), stab, compress);
		ERRORGOTO ("dossier_save_battle_turns()", handle_error);

		hdrs[idx].ra.data = bseekget (fd) - p0;
		hdrs[idx].ra.size = p->racnt * sizeof (SPWAW_DOSSIER_BURA);

		if (hdrs[idx].ra.size) {
			cbio.data = (char *)(p->ra); cbio.size = hdrs[idx].ra.size; cbio.comp = &(hdrs[idx].ra.comp);
			if (!cbwrite (fd, cbio, "compressed battle unit ra", compress))
				FAILGOTO (SPWERR_FWFAILED, "cbwrite(compressed battle unit ra) failed", handle_error);
		}

		rc = dossier_save_battle_map (p, fd, p0, hdrs[idx], compress);
		ERRORGOTO ("dossier_save_battle_map()", handle_error);

		idx++;
	}

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)hdrs, (*cnt) * sizeof (DOS_BHEADER)))
		FAILGOTO (SPWERR_FWFAILED, "bwrite(battle headers)", handle_error);
	bseekset (fd, p1);

	free (hdrs);

	return (SPWERR_OK);

handle_error:
	if (hdrs) free (hdrs);
	*cnt = 0;
	return (rc);
}

static SPWAW_ERROR
dossier_save_campaign_props (SPWAW_DOSSIER *src, DOS_CMPPROPS *props)
{
	CNULLARG(src); CNULLARG (props);

	props->OOB	 = src->props.OOB;
	SPWAW_date2stamp (&(src->props.start), &(props->start));
	SPWAW_date2stamp (&(src->props.end), &(props->end));
	props->maxbtlcnt = src->props.maxbtlcnt;
	props->ifcnt	= src->props.ifcnt;
	props->iucnt	= src->props.iucnt;
	props->cfcnt	= src->props.cfcnt;
	props->cucnt	= src->props.cucnt;

	return (SPWERR_OK);
}

static SPWAW_ERROR
dossier_save_tracking (SPWAW_DOSSIER *src, DOS_TRACKING *tracking)
{
	STRTAB	*stab = NULL;

	CNULLARG(src); CNULLARG (tracking);

	stab = (STRTAB *)src->stab;

	clear_ptr (tracking);

	if ((src->type == SPWAW_MEGACAM_DOSSIER) && !src->tracking.sgd.numeric_id) {
		tracking->gametype	= (ULONG)src->tracking.sgd.gametype;
		tracking->savetype	= (ULONG)src->tracking.sgd.savetype;
		tracking->oobdir	= STRTAB_getidx (stab, src->tracking.sgd.oobdir);
		tracking->path		= STRTAB_getidx (stab, src->tracking.sgd.path);
		tracking->base		= STRTAB_getidx (stab, src->tracking.sgd.id.name);
		tracking->filename	=  STRTAB_getidx (stab, src->tracking.filename);
		tracking->filedate	= *((LONGLONG *)&(src->tracking.filedate));
	}

	return (SPWERR_OK);
}

SPWAW_ERROR
dossier_save (SPWAW_DOSSIER *src, int fd, bool compress)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	long		p0, p1;
	DOS_MV_HEADER	mvhdr;
	DOS_HEADER	hdr;
	STRTAB		*stab = NULL;

	CNULLARG (src);
	stab = (STRTAB *)src->stab;

	STRTAB_compact (stab);

	p0 = bseekget (fd);

	memset (&mvhdr, 0, sizeof (mvhdr));

	memcpy (mvhdr.magic, DOSS_MAGIC, strlen (DOSS_MAGIC));
	mvhdr.version = DOSS_VERSION;

	bseekmove (fd, sizeof (mvhdr));

	memset (&hdr, 0, sizeof (hdr));

	hdr.name = STRTAB_getidx (stab, src->name);
	hdr.comment = STRTAB_getidx (stab, src->comment);
	if (src->savedir) {
		hdr.savedir = STRTAB_getidx (stab, src->savedir);
	} else {
		hdr.savedir = BADSTRIDX;
	}
	hdr.oobdir = STRTAB_getidx (stab, src->oobdir);
	hdr.type = src->type;
	hdr.gametype = src->gametype;

	rc = dossier_save_campaign_props (src, &(hdr.props));	ROE ("dossier_save_campaign_props()");

	bseekmove (fd, sizeof (hdr));

	hdr.oobdata = bseekget (fd) - p0;
	rc = SPWOOB_LIST_compact (src->oobdata);		ROE ("SPWOOB_LIST_compact()");
	rc = SPWOOB_LIST_save (src->oobdata, fd, compress);	ROE ("SPWOOB_LIST_save()");
	SPWOOB_LIST_debug_log (src->oobdata, __FUNCTION__);

	hdr.blist = bseekget (fd) - p0;
	rc = dossier_save_battles (src, fd, &(hdr.bcnt), stab, compress);
	ROE ("dossier_save_battles()");

	hdr.stab = bseekget (fd) - p0;
	rc = STRTAB_fdsave (stab, fd, compress);
	ROE ("STRTAB_fdsave()");

	if (src->type != SPWAW_STDALONE_DOSSIER) {
		hdr.uht = bseekget (fd) - p0;
		rc = UHT_save (&(src->uht), fd, compress);
		ROE ("UHT_fdsave()");
	}

	rc = dossier_save_tracking (src, &(hdr.tracking));	ROE ("dossier_save_tracking()");

	p1 = bseekget (fd); bseekset (fd, p0);
	if (!bwrite (fd, (char *)&mvhdr, sizeof (mvhdr)))
		RWE (SPWERR_FWFAILED, "bwrite(mvhdr) failed");
	if (!bwrite (fd, (char *)&hdr, sizeof (hdr)))
		RWE (SPWERR_FWFAILED, "bwrite(hdr) failed");
	bseekset (fd, p1);

	return (SPWERR_OK);
}
