/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include "snapshot/snapshot.h"
#include "strtab/strtab.h"
#include "common/internal.h"

/* convenience macro */
#define	STRSET(str_)		do { dst->str_ = src->str_; } while (0)

/* convenience macro */
#define	STRCPY(str_)		do { memcpy (dst->str_, src->str_, sizeof (dst->str_)); } while (0)

/* convenience macro */
#define	STABCPY(str_,stab_)	do { dst->str_ = STRTAB_getstr (stab_, STRTAB_getidx (stab_, src->str_)); } while (0)

/* convenience macro */
#define	FREE(ptr_)		do { if (ptr_) { free (ptr_); ptr_ = NULL; } } while (0);

static void
snapcopy_zero (SPWAW_SNAPSHOT *dst)
{
	memset (&(dst->src),	0, sizeof (dst->src));
	memset (&(dst->raw),	0, sizeof (dst->raw));
	memset (&(dst->game),	0, sizeof (dst->game));
	memset (&(dst->OOBp1),	0, sizeof (dst->OOBp1));
	memset (&(dst->OOBp2),	0, sizeof (dst->OOBp2));
}

static SPWAW_ERROR
snapcopy_source (SPWAW_SNAP_SOURCE *src, SPWAW_SNAP_SOURCE *dst, STRTAB *stab)
{
	memcpy (dst, src, sizeof (*dst));

	STABCPY (path, stab);
	STABCPY (file, stab);

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_raw_game (SPWAW_SNAP_GAME_RAW *src, SPWAW_SNAP_GAME_RAW *dst, STRTAB *stab)
{
	memcpy (&(dst->cmt),	  &(src->cmt),	    sizeof (dst->cmt));
	memcpy (&(dst->battle),	  &(src->battle),   sizeof (dst->battle));
	memcpy (&(dst->campaign), &(src->campaign), sizeof (dst->campaign));
	memcpy (&(dst->map),	  &(src->map),	    sizeof (dst->map));

	STABCPY (cmt.title, stab);
	STABCPY (cmt.mapsrc, stab);
	STABCPY (battle.location, stab);

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_raw_oob_formations (SPWAW_SNAP_OOB_FRAW *src, SPWAW_SNAP_OOB_FRAW *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i;

	dst->cnt   = src->cnt;
	dst->start = src->start;

	dst->raw = safe_nmalloc (SPWAW_SNAP_OOB_FELRAW, dst->cnt); COOMGOTO (dst->raw, "SPWAW_SNAP_OOB_FELRAW list", handle_error);
	memcpy (dst->raw, src->raw, dst->cnt * sizeof (SPWAW_SNAP_OOB_FELRAW));
	for (i=0; i<dst->cnt; i++) {
		dst->raw[i].name = STRTAB_getstr (stab, STRTAB_getidx (stab, src->raw[i].name));

	}

	dst->ridx = safe_nmalloc (SPWAW_RINDEX, dst->cnt); COOMGOTO (dst->ridx, "SPWAW_RINDEX list", handle_error);
	memcpy (dst->ridx, src->ridx, dst->cnt * sizeof (SPWAW_RINDEX));

	return (rc);

handle_error:
	FREE (dst->ridx);
	FREE (dst->raw);
	return (rc);
}

static SPWAW_ERROR
snapcopy_raw_oob_units (SPWAW_SNAP_OOB_URAW *src, SPWAW_SNAP_OOB_URAW *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i;

	dst->cnt = src->cnt;

	dst->raw = safe_nmalloc (SPWAW_SNAP_OOB_UELRAW, dst->cnt); COOMGOTO (dst->raw, "SPWAW_SNAP_OOB_UELRAW list", handle_error);
	memcpy (dst->raw, src->raw, dst->cnt * sizeof (SPWAW_SNAP_OOB_UELRAW));
	for (i=0; i<dst->cnt; i++) {
		dst->raw[i].name = STRTAB_getstr (stab, STRTAB_getidx (stab, src->raw[i].name));

	}

	dst->ridx = safe_nmalloc (SPWAW_RINDEX, dst->cnt); COOMGOTO (dst->ridx, "SPWAW_RINDEX list", handle_error);
	memcpy (dst->ridx, src->ridx, dst->cnt * sizeof (SPWAW_RINDEX));

	return (rc);

handle_error:
	FREE (dst->ridx);
	FREE (dst->raw);
	return (rc);
}

static SPWAW_ERROR
snapcopy_raw_oob_leaders (SPWAW_SNAP_OOB_LRAW *src, SPWAW_SNAP_OOB_LRAW *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i;

	dst->cnt = src->cnt;

	dst->raw = safe_nmalloc (SPWAW_SNAP_OOB_LELRAW, dst->cnt); COOMGOTO (dst->raw, "SPWAW_SNAP_OOB_LELRAW list", handle_error);
	memcpy (dst->raw, src->raw, dst->cnt * sizeof (SPWAW_SNAP_OOB_LELRAW));
	for (i=0; i<dst->cnt; i++) {
		dst->raw[i].name = STRTAB_getstr (stab, STRTAB_getidx (stab, src->raw[i].name));

	}

	dst->ridx = safe_nmalloc (SPWAW_RINDEX, dst->cnt); COOMGOTO (dst->ridx, "SPWAW_RINDEX list", handle_error);
	memcpy (dst->ridx, src->ridx, dst->cnt * sizeof (SPWAW_RINDEX));

	return (rc);

handle_error:
	FREE (dst->ridx);
	FREE (dst->raw);
	return (rc);
}

static SPWAW_ERROR
snapcopy_raw_oob_positions (SPWAW_SNAP_OOB_PRAW *src, SPWAW_SNAP_OOB_PRAW *dst, STRTAB * /*stab*/)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i;

	dst->cnt = src->cnt;

	dst->raw = safe_nmalloc (SPWAW_SNAP_OOB_PELRAW, dst->cnt); COOMGOTO (dst->raw, "SPWAW_SNAP_OOB_PELRAW list", handle_error);
	memcpy (dst->raw, src->raw, dst->cnt * sizeof (SPWAW_SNAP_OOB_PELRAW));
	for (i=0; i<dst->cnt; i++);

	dst->ridx = safe_nmalloc (SPWAW_RINDEX, dst->cnt); COOMGOTO (dst->ridx, "SPWAW_RINDEX list", handle_error);
	memcpy (dst->ridx, src->ridx, dst->cnt * sizeof (SPWAW_RINDEX));

	return (rc);

handle_error:
	FREE (dst->ridx);
	FREE (dst->raw);
	return (rc);
}

static SPWAW_ERROR
snapcopy_raw_oob (SPWAW_SNAP_OOB_RAW *src, SPWAW_SNAP_OOB_RAW *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	rc = snapcopy_raw_oob_formations(&(src->formations),	&(dst->formations),	stab);	ROE ("snapcopy_raw_oob_formations()");
	rc = snapcopy_raw_oob_units	(&(src->units),		&(dst->units),		stab);	ROE ("snapcopy_raw_oob_units()");
	rc = snapcopy_raw_oob_leaders	(&(src->leaders),	&(dst->leaders),	stab);	ROE ("snapcopy_raw_oob_leaders()");
	rc = snapcopy_raw_oob_positions	(&(src->positions),	&(dst->positions),	stab);	ROE ("snapcopy_raw_oob_positions()");

	return (rc);
}

static SPWAW_ERROR
snapcopy_raw (SPWAW_SNAP_RAW *src, SPWAW_SNAP_RAW *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	rc = snapcopy_raw_game	(&(src->game),	&(dst->game),	stab);	ROE ("snapcopy_raw_game()");
	rc = snapcopy_raw_oob	(&(src->OOBp1),	&(dst->OOBp1),	stab);	ROE ("snapcopy_raw_oob(OOBp1)");
	rc = snapcopy_raw_oob	(&(src->OOBp2),	&(dst->OOBp2),	stab);	ROE ("snapcopy_raw_oob(OOBp2)");

	return (rc);
}

static SPWAW_ERROR
snapcopy_game_cmt (SPWAW_SNAP_CMT *src, SPWAW_SNAP_CMT *dst, STRTAB *stab)
{
	STABCPY (title, stab);

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_game_battle (SPWAW_SNAP_BATTLE *src, SPWAW_SNAP_BATTLE *dst, STRTAB *stab)
{
	memcpy (&(dst->data), &(src->data), sizeof (src->data));
	STABCPY (data.location, stab);
	memcpy (&(dst->strings), &(src->strings), sizeof (src->strings));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_game_campaign (SPWAW_SNAP_CAMPAIGN *src, SPWAW_SNAP_CAMPAIGN *dst, STRTAB * /*stab*/)
{
	memcpy (&(dst->data), &(src->data), sizeof (src->data));
	memcpy (&(dst->strings), &(src->strings), sizeof (src->strings));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_game_map (SPWAW_SNAP_MAP *src, SPWAW_SNAP_MAP *dst, STRTAB * /*stab*/)
{
	memcpy (dst, src, sizeof (*src));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_game (SPWAW_SNAP_GAME *src, SPWAW_SNAP_GAME *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;

	rc = snapcopy_game_cmt		(&(src->cmt),		&(dst->cmt),		stab);	ROE ("snapcopy_game_cmt()");
	rc = snapcopy_game_battle	(&(src->battle),	&(dst->battle),		stab);	ROE ("snapcopy_game_battle()");
	rc = snapcopy_game_campaign	(&(src->campaign),	&(dst->campaign),	stab);	ROE ("snapcopy_game_campaign()");
	rc = snapcopy_game_map		(&(src->map),		&(dst->map),		stab);	ROE ("snapcopy_game_map()");

	return (rc);
}

static SPWAW_ERROR
snapcopy_oob_attrs (SPWAW_SNAP_OOB_UATTR *src, SPWAW_SNAP_OOB_UATTR *dst, STRTAB * /*stab*/)
{
	memcpy (dst, src, sizeof (*src));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_oob_attrs (SPWAW_SNAP_OOB_FATTR *src, SPWAW_SNAP_OOB_FATTR *dst, STRTAB * /*stab*/)
{
	memcpy (dst, src, sizeof (*src));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_oob_attrs (SPWAW_SNAP_OOB_GATTR *src, SPWAW_SNAP_OOB_GATTR *dst, STRTAB * /*stab*/)
{
	memcpy (dst, src, sizeof (*src));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_oob_formations (SPWAW_SNAP_OOB_F *src, SPWAW_SNAP_OOB_F *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i;

	dst->cnt   = src->cnt;

	dst->list = safe_nmalloc (SPWAW_SNAP_OOB_FEL, dst->cnt); COOMGOTO (dst->list, "SPWAW_SNAP_OOB_FEL list", handle_error);

	memcpy (dst->list, src->list, dst->cnt * sizeof (SPWAW_SNAP_OOB_FEL));
	for (i=0; i<dst->cnt; i++) {
		dst->list[i].data.leader.rid = src->list[i].data.leader.up->data.RID;
		dst->list[i].data.ulist = NULL;

		rc = snapcopy_oob_attrs (&(src->list[i].attr), &(dst->list[i].attr), stab);
		ERRORGOTO ("snapcopy_oob_stats()", handle_error);
	}

	return (rc);

handle_error:
	if (dst->list) {
		for (i=0; i<dst->cnt; i++) {
			FREE (dst->list[i].data.ulist);
		}
		FREE (dst->list);
	}
	return (rc);
}

static SPWAW_ERROR
snapcopy_oob_units (SPWAW_SNAP_OOB_U *src, SPWAW_SNAP_OOB_U *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	DWORD		i;

	dst->cnt   = src->cnt;

	dst->list = safe_nmalloc (SPWAW_SNAP_OOB_UEL, dst->cnt); COOMGOTO (dst->list, "SPWAW_SNAP_OOB_UEL list", handle_error);

	memcpy (dst->list, src->list, dst->cnt * sizeof (SPWAW_SNAP_OOB_UEL));
	for (i=0; i<dst->cnt; i++) {
		dst->list[i].data.formation = NULL;
		if (dst->list[i].data.aunit.up) {
			dst->list[i].data.aunit.rid = dst->list[i].data.aunit.up->data.RID;
		} else {
			dst->list[i].data.aunit.rid = SPWAW_BADIDX;
		}
		if (dst->list[i].data.loader.up) {
			dst->list[i].data.loader.rid = dst->list[i].data.loader.up->data.RID;
		} else {
			dst->list[i].data.loader.rid = SPWAW_BADIDX;
		}

		rc = snapcopy_oob_attrs (&(src->list[i].attr), &(dst->list[i].attr), stab);
		ERRORGOTO ("snapcopy_oob_stats()", handle_error);
	}

	return (rc);

handle_error:
	if (dst->list) {
		FREE (dst->list);
	}
	return (rc);
}

static SPWAW_ERROR
snapcopy_oob_stats (SPWAW_SNAP_OOB_STATS *src, SPWAW_SNAP_OOB_STATS *dst, STRTAB * /*stab*/)
{
	memcpy (dst, src, sizeof (*src));

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_oob_force (SPWAW_SNAP_OOB_FORCE *src, SPWAW_SNAP_OOB_FORCE *dst, STRTAB *stab)
{
	SPWAW_ERROR	rc;

	rc = snapcopy_oob_formations	(&(src->formations),	&(dst->formations),	stab);	ROE ("snapcopy_oob_formations()");
	rc = snapcopy_oob_units		(&(src->units),		&(dst->units),		stab);	ROE ("snapcopy_oob_units(units)");
	rc = snapcopy_oob_units		(&(src->crews),		&(dst->crews),		stab);	ROE ("snapcopy_oob_units(crews)");
	rc = snapcopy_oob_attrs		(&(src->attr),		&(dst->attr),		stab);	ROE ("snapcopy_oob_attrs()");
	rc = snapcopy_oob_stats		(&(src->stats),		&(dst->stats),		stab);	ROE ("snapcopy_oob_stats()");

	return (SPWERR_OK);
}

static SPWAW_ERROR
snapcopy_oob (SPWAW_SNAP_OOB *src, SPWAW_SNAP_OOB *dst, STRTAB *stab, bool sf)
{
	SPWAW_ERROR	rc;

	memcpy (dst, src, sizeof (*src));

	rc = snapcopy_oob_force	(&(src->battle), &(dst->battle), stab);			ROE ("snapcopy_oob_force(battle)");
	if (sf) {
		rc = snapcopy_oob_force	(&(src->core), &(dst->core), stab);		ROE ("snapcopy_oob_force(core)");
		rc = snapcopy_oob_force	(&(src->support), &(dst->support), stab);	ROE ("snapcopy_oob_force(support)");
	}
	rc = OOB_link (dst, sf);							ROE ("OOB_link()");

	return (SPWERR_OK);
}

SPWAW_ERROR
snapcopy (SPWAW_SNAPSHOT *src, SPWAW_SNAPSHOT *dst)
{
	SPWAW_ERROR	rc = SPWERR_OK;
	STRTAB		*stab = NULL;

	CNULLARG (src); CNULLARG (dst);
	snapcopy_zero (dst);

	rc = SPWOOB_copy (dst->oobdat, src->oobdat);
	ROE ("SPWOOB_copy()");

	stab = (STRTAB *)dst->stab;
	rc = STRTAB_merge (stab, (STRTAB *)src->stab);	ROE ("STRTAB_merge()");

	rc = snapcopy_source	(&(src->src),	&(dst->src),	stab);		ROE ("snapcopy_source()");
	rc = snapcopy_raw	(&(src->raw),	&(dst->raw),	stab);		ROE ("snapcopy_raw()");
	rc = snapcopy_game	(&(src->game),	&(dst->game),	stab);		ROE ("snapcopy_game()");
	rc = snapcopy_oob	(&(src->OOBp1),	&(dst->OOBp1),	stab, true);	ROE ("snapcopy_oob(OOBp1)");
	rc = snapcopy_oob	(&(src->OOBp2),	&(dst->OOBp2),	stab, false);	ROE ("snapcopy_oob(OOBp2)");

	return (SPWERR_OK);
}
