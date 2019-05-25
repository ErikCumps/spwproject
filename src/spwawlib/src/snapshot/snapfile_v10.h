/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V10 snapshot.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SNAPFILE_V10_H
#define	INTERNAL_SNAPFILE_V10_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include "snapshot/snapfile.h"
#include "snapshot/snapshot.h"
#include "fileio/fileio.h"

#define	SNAP_VERSION_V10	10

#pragma pack(push, r1, 1)

typedef struct s_SNAP_INFO_V10 {
	ULONG		title;			/* Title symbol								*/
	SPWAW_TIMESTAMP	start;			/* Battle start date/time stamp						*/
	SPWAW_TIMESTAMP	date;			/* Battle turn date/time stamp						*/
	BYTE		turn;			/* Battle turn number							*/
	ULONG		location;		/* Battle location symbol						*/
} SNAP_INFO_V10;

typedef struct s_SNAP_OOB_UEL_V10 {
	USHORT		RID;			/* Unit record ID							*/
	USHORT		FRID;			/* Unit formation record ID						*/
	BYTE		FMID;			/* Unit formation major ID						*/
	BYTE		FSID;			/* Unit formation sub ID						*/
	ULONG		name;			/* Unit name symbol							*/
	BYTE		classID;		/* Unit class ID							*/
	BYTE		OOB;			/* Unit OOB ID								*/
	BYTE		OOBrid;			/* Unit OOB record ID							*/
	BYTE		size;			/* Unit size								*/
	BYTE		cost;			/* Unit cost								*/
	BYTE		survive;		/* Unit survivability							*/
	USHORT		leader;			/* Unit leader number							*/
	BYTE		exp;			/* Unit experience							*/
	BYTE		mor;			/* Unit morale								*/
	BYTE		sup;			/* Unit suppression							*/
	BYTE		status;			/* Unit status								*/
	BYTE		entr;			/* Unit entrenchment status						*/
	BYTE		smkdev;			/* Unit smoke device flag						*/
	BYTE		smkammo;		/* Unit smoke ammo							*/
	USHORT		crew;			/* Unit crew record ID (if abandoned)					*/
	BYTE		range;			/* Unit range								*/
	BYTE		stance_x;		/* Unit stance x coordinate						*/
	BYTE		stance_y;		/* Unit stance y coordinate						*/
	USHORT		loader;			/* Unit loader record ID (if loaded)					*/
	BYTE		load_cap;		/* Unit load capacity							*/
	BYTE		load_cost;		/* Unit load cost							*/
	BYTE		contact;		/* Unit contact status							*/
	BYTE		rof;			/* Unit rate of fire							*/
	BYTE		tgt;			/* Unit targetting rating						*/
	BYTE		rf;			/* Unit rangefinder rating						*/
	BYTE		fc;			/* Unit firecontrol rating						*/
	BYTE		iv;			/* Unit infrared vision rating						*/
	BYTE		swim;			/* Unit swim capability							*/
	BYTE		men;			/* Unit manpower							*/
	BYTE		men_ori;		/* Unit original manpower						*/
	BYTE		speed;			/* Unit speed								*/
	BYTE		moves;			/* Unit available moves							*/
	USHORT		damage;			/* Unit damage								*/
	USHORT		movdir;			/* Unit movement direction (degrees, CW, 0=east)			*/
	USHORT		shtdir;			/* Unit shooting direction (degrees, CW, 0=east)			*/
	USHORT		target;			/* Current target unit record ID					*/
	BYTE		UTGidx;			/* Unit type grouping index						*/
} SNAP_OOB_UEL_V10;

#pragma pack(pop, r1)

extern SPWAW_ERROR	snapshot_load_v10_info_header	(int fd, SNAP_INFO *hdr);
extern SPWAW_ERROR	snapshot_load_v10_oob_uel	(SBR *sbr, SNAP_OOB_UEL *uel);
extern SPWAW_ERROR	snapshot_legacy_ldrcrw_detect	(USHORT cnt, SPWAW_SNAP_OOB_RAW *oob);

#endif	/* INTERNAL_SNAPFILE_V10_H */
