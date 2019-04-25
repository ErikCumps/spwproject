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

#pragma pack(pop, r1)

extern SPWAW_ERROR	snapshot_load_v11_info_header	(int fd, SNAP_INFO *hdr);
extern SPWAW_ERROR	snapshot_load_v11_snap		(int fd, SNAP_HEADER *hdr, SNAP *snap);

#endif	/* INTERNAL_SNAPFILE_V11_H */
