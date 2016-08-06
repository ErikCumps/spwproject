/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "stdafx.h"
#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>
#include "common/internal.h"

SPWAW_ERROR
build_fridx (SPWAW_SNAP_OOB_FRAW *src)
{
	USHORT	i;

	if (src->ridx) free (src->ridx);

	src->ridx = safe_nmalloc (SPWAW_RINDEX, src->cnt); COOM (src->ridx, "SPWAW_RINDEX list");

	for (i=0; i<src->cnt; i++) {
		src->ridx[i].rid = src->raw[i].RID;
		src->ridx[i].idx = i;
	}
	return (SPWERR_OK);
}

SPWAW_ERROR
build_uridx (SPWAW_SNAP_OOB_URAW *src)
{
	USHORT	i;

	if (src->ridx) free (src->ridx);

	src->ridx = safe_nmalloc (SPWAW_RINDEX, src->cnt); COOM (src->ridx, "SPWAW_RINDEX list");

	for (i=0; i<src->cnt; i++) {
		src->ridx[i].rid = src->raw[i].RID;
		src->ridx[i].idx = i;
	}
	return (SPWERR_OK);
}

SPWAW_ERROR
build_lridx (SPWAW_SNAP_OOB_LRAW *src)
{
	USHORT	i;

	if (src->ridx) free (src->ridx);

	src->ridx = safe_nmalloc (SPWAW_RINDEX, src->cnt); COOM (src->ridx, "SPWAW_RINDEX list");

	for (i=0; i<src->cnt; i++) {
		src->ridx[i].rid = src->raw[i].RID;
		src->ridx[i].idx = i;
	}
	return (SPWERR_OK);
}

SPWAW_ERROR
build_pridx (SPWAW_SNAP_OOB_PRAW *src)
{
	USHORT	i;

	if (src->ridx) free (src->ridx);

	src->ridx = safe_nmalloc (SPWAW_RINDEX, src->cnt); COOM (src->ridx, "SPWAW_RINDEX list");

	for (i=0; i<src->cnt; i++) {
		src->ridx[i].rid = src->raw[i].RID;
		src->ridx[i].idx = i;
	}
	return (SPWERR_OK);
}

USHORT
fridx (SPWAW_SNAP_OOB_FRAW *src, USHORT cri)
{
	USHORT	i, idx = SPWAW_BADIDX;

	if (!src->ridx) return (idx);

	for (i=0; i<src->cnt; i++) {
		if (src->ridx[i].rid == cri) {
			idx = src->ridx[i].idx;
			break;
		}
	}
	return (idx);
}

USHORT
uridx (SPWAW_SNAP_OOB_URAW *src, USHORT cri)
{
	USHORT	i, idx = SPWAW_BADIDX;

	if (!src->ridx) return (idx);

	for (i=0; i<src->cnt; i++) {
		if (src->ridx[i].rid == cri) {
			idx = src->ridx[i].idx;
			break;
		}
	}
	return (idx);
}

USHORT
lridx (SPWAW_SNAP_OOB_LRAW *src, USHORT cri)
{
	USHORT	i, idx = SPWAW_BADIDX;

	if (!src->ridx) return (idx);

	for (i=0; i<src->cnt; i++) {
		if (src->ridx[i].rid == cri) {
			idx = src->ridx[i].idx;
			break;
		}
	}
	return (idx);
}

USHORT
pridx (SPWAW_SNAP_OOB_PRAW *src, USHORT cri)
{
	USHORT	i, idx = SPWAW_BADIDX;

	if (!src->ridx) return (idx);

	for (i=0; i<src->cnt; i++) {
		if (src->ridx[i].rid == cri) {
			idx = src->ridx[i].idx;
			break;
		}
	}
	return (idx);
}
