/** \file
 * The SPWaW Library - snapshot handling - backwards compatibility with the V11 snapshot.
 *
 * Copyright (C) 2018-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SNAPFILE_V11_H
#define	INTERNAL_SNAPFILE_V11_H	1

#include <spwawlib_api.h>
#include "snapshot/snapfile.h"
#include "fileio/fileio.h"

#define	SNAP_VERSION_V11	11

#pragma pack(push, r1, 1)

typedef struct s_SNAP_INFO_V11 {
	ULONG		title;			/* Title symbol								*/
	SPWAW_TIMESTAMP	start;			/* Battle start date/time stamp						*/
	SPWAW_TIMESTAMP	date;			/* Battle turn date/time stamp						*/
	BYTE		turn;			/* Battle turn number							*/
	ULONG		location;		/* Battle location symbol						*/
	ULONG		type;			/* Snapshot battle type							*/
} SNAP_INFO_V11;

typedef struct s_SNAP_CMT_V11 {
	ULONG			title;			/* Savegame title symbol						*/
	ULONG			mapsrc;			/* Savegame map source symbol						*/
} SNAP_CMT_V11;

typedef struct s_SNAP_VHEX_V11 {
	BYTE			x;			/* Victory hex x-coord							*/
	BYTE			y;			/* Victory hex y-coord							*/
	BYTE			value;			/* Victory hex point value						*/
	BYTE			owner;			/* Victory hex owner							*/
} SNAP_VHEX_V11;

typedef struct s_SNAP_BATTLE_V11 {
	BYTE			year;			/* Battle year								*/
	BYTE			month;			/* Battle month								*/
	BYTE			day;			/* Battle day								*/
	BYTE			hour;			/* Battle hour								*/
	ULONG			location;		/* Battle location symbol						*/
	BYTE			terrain;		/* Battle terrain type							*/
	BYTE			weather;		/* Battle weather type							*/
	BYTE			visibility;		/* Battle visibility							*/
	BYTE			turn;			/* Battle turn								*/
	BYTE			turn_max;		/* Battle max turn							*/
	BYTE			OOB_p1;			/* Player1 OOB ID							*/
	BYTE			OOB_p2;			/* Player2 OOB ID							*/
	BYTE			OOB_p3;			/* Player3 OOB ID							*/
	BYTE			OOB_p4;			/* Player4 OOB ID							*/
	BYTE			miss_p1;		/* Mission type player1							*/
	BYTE			miss_p2;		/* Mission type player2							*/
	DWORD			credit;			/* Purchase credit points						*/
	SNAP_VHEX_V11		vhex[SNAP_VHEXCNT];	/* Victory hexes							*/
} SNAP_BATTLE_V11;

typedef struct s_SNAP_CAMPAIGN_V11 {
	BYTE			campaign;		/* Campaign mode flag?							*/
	USHORT			start_year;		/* Campaign start year							*/
	USHORT			start_month;		/* Campaign start month							*/
	USHORT			end_year;		/* Campaign start year							*/
	USHORT			end_month;		/* Campaign start month							*/
	USHORT			battles;		/* Battles fought							*/
	USHORT			majvics;		/* Major victories							*/
	USHORT			minvics;		/* Minor victories							*/
	USHORT			battles_max;		/* Max battles								*/
	USHORT			P1BLmen;		/* Player1 battle losses: men						*/
	USHORT			P1BLart;		/* Player1 battle losses: ART						*/
	USHORT			P1BLsoft;		/* Player1 battle losses: soft vehicles					*/
	USHORT			P1BLapc;		/* Player1 battle losses: APC						*/
	USHORT			P1BLafv;		/* Player1 battle losses: AFV						*/
	USHORT			P1BLgliders;		/* Player1 battle losses: gliders					*/
	USHORT			P1BLair;		/* Player1 battle losses: aircraft					*/
	USHORT			P2BLmen;		/* Player2 battle losses: men						*/
	USHORT			P2BLart;		/* Player2 battle losses: ART						*/
	USHORT			P2BLsoft;		/* Player2 battle losses: soft vehicles					*/
	USHORT			P2BLapc;		/* Player2 battle losses: APC						*/
	USHORT			P2BLafv;		/* Player2 battle losses: AFV						*/
	USHORT			P2BLgliders;		/* Player2 battle losses: gliders					*/
	USHORT			P2BLair;		/* Player2 battle losses: aircraft					*/
	USHORT			P1TLmen;		/* Player1 campaign total losses: men					*/
	USHORT			P1TLart;		/* Player1 campaign total losses: ART					*/
	USHORT			P1TLsoft;		/* Player1 campaign total losses: soft vehicles				*/
	USHORT			P1TLapc;		/* Player1 campaign total losses: APC					*/
	USHORT			P1TLafv;		/* Player1 campaign total losses: AFV					*/
	USHORT			P1TLgliders;		/* Player1 campaign total losses: gliders				*/
	USHORT			P1TLair;		/* Player1 campaign total losses: aircraft				*/
	USHORT			P2TLmen;		/* Player2 campaign total losses: men					*/
	USHORT			P2TLart;		/* Player2 campaign total losses: ART					*/
	USHORT			P2TLsoft;		/* Player2 campaign total losses: soft vehicles				*/
	USHORT			P2TLapc;		/* Player2 campaign total losses: APC					*/
	USHORT			P2TLafv;		/* Player2 campaign total losses: AFV					*/
	USHORT			P2TLgliders;		/* Player2 campaign total losses: gliders				*/
	USHORT			P2TLair;		/* Player2 campaign total losses: aircraft				*/
	USHORT			busy;			/* Inverted battle busy flag						*/
	DWORD			P1score;		/* Player1 battle score							*/
	DWORD			P2score;		/* Player2 battle score							*/
} SNAP_CAMPAIGN_V11;

typedef struct s_SNAP_V11 {
	SNAP_CMT_V11		cmt;			/* Snapshot comment data						*/
	SNAP_BATTLE_V11		b;			/* Snapshot battle data							*/
	SNAP_CAMPAIGN_V11	c;			/* Snapshot campaign data						*/
} SNAP_V11;

typedef struct s_SNAP_MAPDATA_V11 {
	SHORT		height;			/* Hex height								*/
	BYTE		has_T1;			/* Hex terrain features (set #1)					*/
	BYTE		has_T2;			/* Hex terrain features (set #2)					*/
	BYTE		has_T3;			/* Hex terrain features (set #3)					*/
	BYTE		has_T4;			/* Hex terrain features (set #4)					*/
	BYTE		conn_road1;		/* Hex primary road connections						*/
	BYTE		conn_road2;		/* Hex secondary road connections					*/
	BYTE		conn_rail;		/* Hex railroad connections						*/
} SNAP_MAPDATA_V11;

typedef struct s_SNAP_OOBHDR_V11 {
	USHORT		fcnt;			/* Formation count							*/
	ULONG		fpos;			/* Formation data offset, relative to start of header			*/
	ULONG		fsize;			/* Formation data size							*/
	ULONG		fcomp;			/* Formation compressed data size (0 if no compression)			*/
	USHORT		fstart;			/* Formation ID start							*/
	USHORT		ucnt;			/* Unit count								*/
	ULONG		upos;			/* Unit data offset, relative to start of header			*/
	ULONG		usize;			/* Unit data size							*/
	ULONG		ucomp;			/* Unit compressed data size (0 if no compression)			*/
	USHORT		lcnt;			/* Leader count								*/
	ULONG		lpos;			/* Leader data offset, relative to start of header			*/
	ULONG		lsize;			/* Leader data size							*/
	ULONG		lcomp;			/* Leader compressed data size (0 if no compression)			*/
	USHORT		pcnt;			/* Position count							*/
	ULONG		ppos;			/* Position data offset, relative to start of header			*/
	ULONG		psize;			/* Position data size							*/
	ULONG		pcomp;			/* Position compressed data size (0 if no compression)			*/
} SNAP_OOBHDR_V11;

typedef struct s_SNAP_OOB_FEL_V11 {
	USHORT		RID;			/* Formation record ID							*/
	BYTE		FID;			/* Formation ID								*/
	ULONG		name;			/* Formation name symbol						*/
	USHORT		leader;			/* Formation leader unit record ID					*/
	USHORT		hcmd;			/* Formation higher command leader unit record ID			*/
	BYTE		OOBrid;			/* Formation OOB record ID						*/
	BYTE		status;			/* Formation campaign status						*/
} SNAP_OOB_FEL_V11;

typedef struct s_SNAP_OOB_UEL_V11 {
	BYTE		dutype;			/* Detected unit type							*/
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
	BYTE		aband;			/* Unit abandonment status						*/
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
} SNAP_OOB_UEL_V11;

typedef struct s_SNAP_OOB_LEL_V11 {
	USHORT		RID;			/* Leader record ID							*/
	USHORT		URID;			/* Leader unit record ID						*/
	ULONG		name;			/* Leader name symbol							*/
	BYTE		rank;			/* Leader rank								*/
	BYTE		ral;			/* Leader rally skill							*/
	BYTE		inf;			/* Leader infantry skill						*/
	BYTE		art;			/* Leader artillery skill						*/
	BYTE		arm;			/* Leader armor skill							*/
	BYTE		kills;			/* Leader kill count							*/
	BYTE		status;			/* Leader status							*/
} SNAP_OOB_LEL_V11;
#pragma pack(pop, r1)

extern SPWAW_ERROR	snapshot_load_v11_info_header	(int fd, SNAP_INFO *hdr);
extern SPWAW_ERROR	snapshot_load_v11_snap		(int fd, SNAP_HEADER *hdr, SNAP *snap);
extern SPWAW_ERROR	snapshot_load_v11_map_data	(SBR *sbr, SNAP_MAPDATA *data);
extern SPWAW_ERROR	snapshot_load_v11_oob_header	(int fd, SNAP_OOBHDR *hdr);
extern SPWAW_ERROR	snapshot_load_v11_oob_fel	(SBR *sbr, SNAP_OOB_FEL *fel);
extern SPWAW_ERROR	snapshot_load_v11_oob_uel	(SBR *sbr, SNAP_OOB_UEL *uel);
extern SPWAW_ERROR	snapshot_load_v11_oob_lel	(SBR *sbr, SNAP_OOB_LEL *lel);

#endif	/* INTERNAL_SNAPFILE_V11_H */
