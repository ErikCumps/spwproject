/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SNAPFILE_H
#define	INTERNAL_SNAPFILE_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>
#include <spwawlib_defines.h>

#define	SNAP_MAGIC	"SPWAWLIB_SNAPSHOT"
#define	SNAP_MGCLEN	20
#define	SNAP_VERSION	14
#define	SNAP_VHEXCNT	SPWAW_VHEXCNT

#pragma pack(push, r1, 1)

typedef struct s_SNAP_HEADER {
	char		magic[SNAP_MGCLEN];	/* Snapshot magic string						*/
	ULONG		version;		/* Snapshot file format version						*/
	ULONG		src;			/* Snapshot source data offset, relative to start of header		*/
	ULONG		oobdat;			/* SPWOOB data offset, relative to start of header, 0 if not present	*/
	ULONG		info;			/* Snapshot info data offset, relative to start of header		*/
	struct s_snap {				/* Snapshot game data:							*/
		ULONG	data;			/* data offset, relative to start of header				*/
		ULONG	size;			/* data size								*/
		ULONG	comp;			/* compressed data size (0 if no compression)				*/
	}	snap;
	ULONG		map;			/* Snapshot map data offset, relative to start of header		*/
	ULONG		oobp1;			/* Player1 OOB data offset, relative to start of header			*/
	ULONG		oobp2;			/* Player2 OOB data offset, relative to start of header			*/
	ULONG		stab;			/* String table offset, relative to start of header, 0 if not present	*/
} SNAP_HEADER;

typedef struct s_SNAP_SOURCE {
	ULONG		path;			/* Source game file path symbol						*/
	ULONG		file;			/* Source game file name symbol						*/
	LONGLONG	date;			/* Source game file modification date					*/
} SNAP_SOURCE;

typedef struct s_SNAP_INFO {
	ULONG		title;			/* Title symbol								*/
	SPWAW_TIMESTAMP	start;			/* Battle start date/time stamp						*/
	SPWAW_TIMESTAMP	date;			/* Battle turn date/time stamp						*/
	BYTE		turn;			/* Battle turn number							*/
	ULONG		location;		/* Battle location symbol						*/
	ULONG		type;			/* Snapshot battle type							*/
	ULONG		gametype;		/* Snapshot game type							*/
	ULONG		savetype;		/* Snapshot save type							*/
} SNAP_INFO;

typedef struct s_SNAP_META {
	ULONG		title;			/* Savegame title symbol						*/
	ULONG		mapsrc;			/* Savegame map source symbol						*/
} SNAP_META;

typedef struct s_SNAP_VHEX {
	BYTE		x;			/* Victory hex x-coord							*/
	BYTE		y;			/* Victory hex y-coord							*/
	BYTE		value;			/* Victory hex point value						*/
	BYTE		owner;			/* Victory hex owner							*/
} SNAP_VHEX;

typedef struct s_SNAP_BATTLE {
	BYTE		year;			/* Battle year								*/
	BYTE		month;			/* Battle month								*/
	BYTE		day;			/* Battle day								*/
	BYTE		hour;			/* Battle hour								*/
	ULONG		location;		/* Battle location symbol						*/
	BYTE		terrain;		/* Battle terrain type							*/
	BYTE		weather;		/* Battle weather type							*/
	BYTE		visibility;		/* Battle visibility							*/
	BYTE		turn;			/* Battle turn								*/
	BYTE		turn_max;		/* Battle max turn							*/
	BYTE		OOB_p1;			/* Player1 OOB ID							*/
	BYTE		OOB_p2;			/* Player2 OOB ID							*/
	BYTE		OOB_p3;			/* Player3 OOB ID							*/
	BYTE		OOB_p4;			/* Player4 OOB ID							*/
	BYTE		miss_p1;		/* Mission type player1							*/
	BYTE		miss_p2;		/* Mission type player2							*/
	DWORD		credit;			/* Purchase credit points						*/
	SNAP_VHEX	vhex[SNAP_VHEXCNT];	/* Victory hexes							*/
} SNAP_BATTLE;

typedef struct s_SNAP_CAMPAIGN {
	BYTE		campaign;		/* Campaign mode flag?							*/
	USHORT		start_year;		/* Campaign start year							*/
	USHORT		start_month;		/* Campaign start month							*/
	USHORT		end_year;		/* Campaign start year							*/
	USHORT		end_month;		/* Campaign start month							*/
	USHORT		battles;		/* Battles fought							*/
	USHORT		majvics;		/* Major victories							*/
	USHORT		minvics;		/* Minor victories							*/
	USHORT		battles_max;		/* Max battles								*/
	DWORD		P1BLmen;		/* Player1 battle losses: men						*/
	DWORD		P1BLart;		/* Player1 battle losses: ART						*/
	DWORD		P1BLsoft;		/* Player1 battle losses: soft vehicles					*/
	DWORD		P1BLapc;		/* Player1 battle losses: APC						*/
	DWORD		P1BLafv;		/* Player1 battle losses: AFV						*/
	DWORD		P1BLgliders;		/* Player1 battle losses: gliders					*/
	DWORD		P1BLair;		/* Player1 battle losses: aircraft					*/
	DWORD		P2BLmen;		/* Player2 battle losses: men						*/
	DWORD		P2BLart;		/* Player2 battle losses: ART						*/
	DWORD		P2BLsoft;		/* Player2 battle losses: soft vehicles					*/
	DWORD		P2BLapc;		/* Player2 battle losses: APC						*/
	DWORD		P2BLafv;		/* Player2 battle losses: AFV						*/
	DWORD		P2BLgliders;		/* Player2 battle losses: gliders					*/
	DWORD		P2BLair;		/* Player2 battle losses: aircraft					*/
	USHORT		P1TLmen;		/* Player1 campaign total losses: men					*/
	DWORD		P1TLart;		/* Player1 campaign total losses: ART					*/
	DWORD		P1TLsoft;		/* Player1 campaign total losses: soft vehicles				*/
	DWORD		P1TLapc;		/* Player1 campaign total losses: APC					*/
	DWORD		P1TLafv;		/* Player1 campaign total losses: AFV					*/
	DWORD		P1TLgliders;		/* Player1 campaign total losses: gliders				*/
	DWORD		P1TLair;		/* Player1 campaign total losses: aircraft				*/
	USHORT		P2TLmen;		/* Player2 campaign total losses: men					*/
	DWORD		P2TLart;		/* Player2 campaign total losses: ART					*/
	DWORD		P2TLsoft;		/* Player2 campaign total losses: soft vehicles				*/
	DWORD		P2TLapc;		/* Player2 campaign total losses: APC					*/
	DWORD		P2TLafv;		/* Player2 campaign total losses: AFV					*/
	DWORD		P2TLgliders;		/* Player2 campaign total losses: gliders				*/
	DWORD		P2TLair;		/* Player2 campaign total losses: aircraft				*/
	USHORT		busy;			/* Inverted battle busy flag						*/
	DWORD		P1score;		/* Player1 battle score							*/
	DWORD		P2score;		/* Player2 battle score							*/
	SHORT		P1result;		/* Player1 battle result						*/
	SHORT		P2result;		/* Player2 battle result						*/
} SNAP_CAMPAIGN;

typedef struct s_SNAP {
	SNAP_META	meta;			/* Snapshot meta data							*/
	SNAP_BATTLE	b;			/* Snapshot battle data							*/
	SNAP_CAMPAIGN	c;			/* Snapshot campaign data						*/
} SNAP;

typedef struct s_SNAP_MAPHDR {
	DWORD		width;			/* Battle map width							*/
	DWORD		height;			/* Battle map height							*/
	ULONG		data;			/* Map data offset, relative to start of header				*/
	ULONG		size;			/* data size								*/
	ULONG		comp;			/* compressed data size (0 if no compression)				*/
} SNAP_MAPHDR;

typedef struct s_SNAP_MAPDATA {
	SHORT		height;			/* Hex height								*/
	BYTE		has_T1;			/* Hex terrain features (set #1)					*/
	BYTE		has_T2;			/* Hex terrain features (set #2)					*/
	BYTE		has_T3;			/* Hex terrain features (set #3)					*/
	BYTE		has_T4;			/* Hex terrain features (set #4)					*/
	ULONGLONG	tfs;			/* Hex terrain (classified) feature set					*/
	BYTE		conn_road1;		/* Hex primary road connections						*/
	BYTE		conn_road2;		/* Hex secondary road connections					*/
	BYTE		conn_rail;		/* Hex railroad connections						*/
	BYTE		conn_tram;		/* Hex tramline connections						*/
} SNAP_MAPDATA;

typedef struct s_SNAP_OOB_FEL {
	USHORT		RID;			/* Formation record ID							*/
	USHORT		FID;			/* Formation ID								*/
	ULONG		name;			/* Formation name symbol						*/
	USHORT		leader;			/* Formation leader unit record ID					*/
	USHORT		hcmd;			/* Formation higher command leader unit record ID			*/
	USHORT		OOBrid;			/* Formation OOB record ID						*/
	BYTE		status;			/* Formation campaign status						*/
} SNAP_OOB_FEL;

typedef struct s_SNAP_OOB_UEL {
	BYTE		dutype;			/* Detected unit type							*/
	USHORT		RID;			/* Unit record ID							*/
	USHORT		FRID;			/* Unit formation record ID						*/
	USHORT		FMID;			/* Unit formation major ID						*/
	BYTE		FSID;			/* Unit formation sub ID						*/
	ULONG		name;			/* Unit name symbol							*/
	BYTE		classID;		/* Unit class ID							*/
	BYTE		OOB;			/* Unit OOB ID								*/
	USHORT		OOBrid;			/* Unit OOB record ID							*/
	BYTE		size;			/* Unit size								*/
	USHORT		cost;			/* Unit cost								*/
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
	BYTE		ew;			/* Unit Electronic Warfare rating					*/
	BYTE		men;			/* Unit manpower							*/
	BYTE		men_ori;		/* Unit original manpower						*/
	BYTE		speed;			/* Unit speed								*/
	BYTE		moves;			/* Unit available moves							*/
	USHORT		damage;			/* Unit damage								*/
	USHORT		movdir;			/* Unit movement direction (degrees, CW, 0=east)			*/
	USHORT		shtdir;			/* Unit shooting direction (degrees, CW, 0=east)			*/
	USHORT		target;			/* Current target unit record ID					*/
	BYTE		UTGidx;			/* Unit type grouping index						*/
} SNAP_OOB_UEL;

typedef struct s_SNAP_OOB_LEL {
	USHORT		RID;			/* Leader record ID							*/
	USHORT		URID;			/* Leader unit record ID						*/
	ULONG		name;			/* Leader name symbol							*/
	BYTE		rank;			/* Leader rank								*/
	BYTE		ral;			/* Leader rally skill							*/
	BYTE		inf;			/* Leader infantry skill						*/
	BYTE		art;			/* Leader artillery skill						*/
	BYTE		arm;			/* Leader armor skill							*/
	USHORT		kills;			/* Leader kill count							*/
	BYTE		status;			/* Leader status							*/
} SNAP_OOB_LEL;

typedef struct s_SNAP_OOB_PEL {
	USHORT		RID;			/* Position record ID							*/
	USHORT		URID;			/* Position unit record ID						*/
	USHORT		x;			/* Position x-coord							*/
	USHORT		y;			/* Position y-coord							*/
	BYTE		seen;			/* Unit spotted flag							*/
} SNAP_OOB_PEL;

typedef struct s_SNAP_OOBHDR {
	USHORT		fcnt;			/* Formation count							*/
	ULONG		fpos;			/* Formation data offset, relative to start of header			*/
	ULONG		fsize;			/* Formation data size							*/
	ULONG		fcomp;			/* Formation compressed data size (0 if no compression)			*/
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
} SNAP_OOBHDR;

#pragma pack(pop, r1)

#endif	/* INTERNAL_SNAPFILE_H */
