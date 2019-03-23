/** \file
 * The SPWaW Library - snapshot API.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_SNAPSHOT_H
#define	SPWAW_LIB_SNAPSHOT_H	1

#include <spwawlib_api.h>
#include <spwawlib_defines.h>
#include <spwawlib_types.h>
#include <spwawlib_ud.h>
#include <spwawlib_spwoob.h>

/*******************************/
/***   RAW DATA STRUCTURES   ***/
/*******************************/

/* SPWAW comment info: raw data */
typedef struct s_SPWAW_SNAP_CMT_RAW {
	char			*title;				/* Savegame title					*/
	char			*mapsrc;			/* Savegame map source					*/
}SPWAW_SNAP_CMT_RAW;

/* SPWAW battle info: raw victory hex data */
typedef struct s_SPWAW_SNAP_VHEX_RAW {
	BYTE			x;				/* Victory hex x-coord					*/
	BYTE			y;				/* Victory hex y-coord					*/
	BYTE			value;				/* Victory hex point value				*/
	BYTE			owner;				/* Victory hex owner					*/
} SPWAW_SNAP_VHEX_RAW;

/* SPWAW battle info: raw data */
typedef struct s_SPWAW_SNAP_BATTLE_RAW {
	BYTE			year;				/* Battle year						*/
	BYTE			month;				/* Battle month						*/
	BYTE			day;				/* Battle day						*/
	BYTE			hour;				/* Battle hour						*/
	char			*location;			/* Battle location name					*/
	BYTE			terrain;			/* Battle terrain type					*/
	BYTE			weather;			/* Battle weather type					*/
	BYTE			visibility;			/* Battle visibility					*/
	BYTE			turn;				/* Battle turn						*/
	BYTE			turn_max;			/* Battle max turn					*/
	BYTE			OOB_p1;				/* Player1 OOB ID					*/
	BYTE			OOB_p2;				/* Player2 OOB ID					*/
	BYTE			OOB_p3;				/* Player3 OOB ID					*/
	BYTE			OOB_p4;				/* Player4 OOB ID					*/
	BYTE			miss_p1;			/* Mission type player1					*/
	BYTE			miss_p2;			/* Mission type player2					*/
	DWORD			credit;				/* Purchase credit points				*/
	SPWAW_SNAP_VHEX_RAW	vhex[SPWAW_VHEXCNT];		/* Victory hexes					*/
} SPWAW_SNAP_BATTLE_RAW;

/* SPWAW campaign info: raw data */
typedef struct s_SPWAW_SNAP_CAMPAIGN_RAW {
	BYTE			campaign;			/* Campaign mode flag?					*/
	USHORT			start_year;			/* Campaign start year					*/
	USHORT			start_month;			/* Campaign start month					*/
	USHORT			end_year;			/* Campaign start year					*/
	USHORT			end_month;			/* Campaign start month					*/
	USHORT			battles;			/* Battles fought					*/
	USHORT			majvics;			/* Major victories					*/
	USHORT			minvics;			/* Minor victories					*/
	USHORT			battles_max;			/* Max battles						*/
	DWORD			P1BLmen;			/* Player1 battle losses: men				*/
	DWORD			P1BLart;			/* Player1 battle losses: ART				*/
	DWORD			P1BLsoft;			/* Player1 battle losses: soft vehicles			*/
	DWORD			P1BLapc;			/* Player1 battle losses: APC				*/
	DWORD			P1BLafv;			/* Player1 battle losses: AFV				*/
	DWORD			P1BLgliders;			/* Player1 battle losses: gliders			*/
	DWORD			P1BLair;			/* Player1 battle losses: aircraft			*/
	DWORD			P2BLmen;			/* Player2 battle losses: men				*/
	DWORD			P2BLart;			/* Player2 battle losses: ART				*/
	DWORD			P2BLsoft;			/* Player2 battle losses: soft vehicles			*/
	DWORD			P2BLapc;			/* Player2 battle losses: APC				*/
	DWORD			P2BLafv;			/* Player2 battle losses: AFV				*/
	DWORD			P2BLgliders;			/* Player2 battle losses: gliders			*/
	DWORD			P2BLair;			/* Player2 battle losses: aircraft			*/
	USHORT			P1TLmen;			/* Player1 campaign total losses: men			*/
	DWORD			P1TLart;			/* Player1 campaign total losses: ART			*/
	DWORD			P1TLsoft;			/* Player1 campaign total losses: soft vehicles		*/
	DWORD			P1TLapc;			/* Player1 campaign total losses: APC			*/
	DWORD			P1TLafv;			/* Player1 campaign total losses: AFV			*/
	DWORD			P1TLgliders;			/* Player1 campaign total losses: gliders		*/
	DWORD			P1TLair;			/* Player1 campaign total losses: aircraft		*/
	USHORT			P2TLmen;			/* Player2 campaign total losses: men			*/
	DWORD			P2TLart;			/* Player2 campaign total losses: ART			*/
	DWORD			P2TLsoft;			/* Player2 campaign total losses: soft vehicles		*/
	DWORD			P2TLapc;			/* Player2 campaign total losses: APC			*/
	DWORD			P2TLafv;			/* Player2 campaign total losses: AFV			*/
	DWORD			P2TLgliders;			/* Player2 campaign total losses: gliders		*/
	DWORD			P2TLair;			/* Player2 campaign total losses: aircraft		*/
	USHORT			busy;				/* Inverted battle busy flag				*/
	DWORD			P1score;			/* Player1 battle score					*/
	DWORD			P2score;			/* Player2 battle score					*/
	SHORT			P1result;			/* Player1 battle result				*/
	SHORT			P2result;			/* Player2 battle result				*/
} SPWAW_SNAP_CAMPAIGN_RAW;

/* SPWAW map info: raw map data element */
typedef struct s_SPWAW_SNAP_MAP_DRAW {
	SHORT			height;				/* hex height						*/
	BYTE			has_T1;				/* hex terrain features (set #1)			*/
	BYTE			has_T2;				/* hex terrain features (set #2)			*/
	BYTE			has_T3;				/* hex terrain features (set #3)			*/
	BYTE			has_T4;				/* hex terrain features (set #4)			*/
	BYTE			conn_road1;			/* hex primary road connections				*/
	BYTE			conn_road2;			/* hex secondary road connections			*/
	BYTE			conn_rail;			/* hex railroad connections				*/
	BYTE			conn_tram;			/* hex tramline connections				*/
} SPWAW_SNAP_MAP_DRAW;

/* SPWAW map info: raw data */
typedef struct s_SPWAW_SNAP_MAP_RAW {
	DWORD			width;				/* Battle map width					*/
	DWORD			height;				/* Battle map height					*/
	SPWAW_SNAP_MAP_DRAW	*data;				/* Battle map data					*/
	DWORD			size;				/* Battle map data size					*/
} SPWAW_SNAP_MAP_RAW;

/* SPWAW game info: raw data */
typedef struct s_SPWAW_SNAP_GAME_RAW {
	SPWAW_SNAP_CMT_RAW	cmt;				/* Raw comment data					*/
	SPWAW_SNAP_BATTLE_RAW	battle;				/* Raw battle data					*/
	SPWAW_SNAP_CAMPAIGN_RAW	campaign;			/* Raw campaign data					*/
	SPWAW_SNAP_MAP_RAW	map;				/* Raw map data						*/
	SPWAW_UD		UD;				/* Game UD						*/
} SPWAW_SNAP_GAME_RAW;

/* SPWAW: record index element */
typedef struct s_SPWAW_RINDEX {
	USHORT			rid;
	USHORT			idx;
} SPWAW_RINDEX;

/* SPWAW: raw formation element */
typedef struct s_SPWAW_SNAP_OOB_FELRAW {
	USHORT			RID;				/* Formation record ID					*/
	USHORT			FID;				/* Formation ID						*/
	char			*name;				/* Formation name					*/
	USHORT			leader;				/* Formation leader unit record ID			*/
	USHORT			hcmd;				/* Formation higher command leader unit record ID	*/
	USHORT			OOBrid;				/* Formation OOB record ID				*/
	BYTE			status;				/* Formation campaign status				*/
	USHORT			player;				/* Formation player flag				*/
	SPWAW_UD		UD;				/* Formation UD						*/
} SPWAW_SNAP_OOB_FELRAW;

/* SPWAW OOB: formation raw data */
typedef struct s_SPWAW_SNAP_OOB_FRAW {
	USHORT			cnt;				/* raw data count					*/
	SPWAW_SNAP_OOB_FELRAW	*raw;				/* array of raw data elements				*/
	SPWAW_RINDEX		*ridx;				/* record index array					*/
	USHORT			start;				/* formation ID start number				*/
} SPWAW_SNAP_OOB_FRAW;

/* SPWAW: raw unit element */
typedef struct s_SPWAW_SNAP_OOB_UELRAW {
	SPWAW_UNIT_TYPE		dutype;				/* Detected unit type					*/
	USHORT			RID;				/* Unit record ID					*/
	USHORT			FRID;				/* Unit formation record ID				*/
	USHORT			FMID;				/* Unit formation major ID				*/
	BYTE			FSID;				/* Unit formation sub ID				*/
	char			*name;				/* Unit formation name					*/
	BYTE			classID;			/* Unit class ID					*/
	BYTE			OOB;				/* Unit OOB ID						*/
	USHORT			OOBrid;				/* Unit OOB record ID					*/
	BYTE			size;				/* Unit size						*/
	BYTE			cost;				/* Unit cost						*/
	BYTE			survive;			/* Unit survivability					*/
	USHORT			leader;				/* Detected unit leader record ID			*/
	BYTE			exp;				/* Unit experience					*/
	BYTE			mor;				/* Unit morale						*/
	BYTE			sup;				/* Unit suppression					*/
	BYTE			status;				/* Unit status						*/
	BYTE			entr;				/* Unit entrenchment status				*/
	SPWAW_ABAND		aband;				/* Unit abandonment status				*/
	BYTE			smkdev;				/* Unit smoke device flag				*/
	BYTE			smkammo;			/* Unit smoke ammo					*/
	USHORT			crew;				/* Unit crew record ID (if abandoned)			*/
	BYTE			range;				/* Unit range						*/
	BYTE			stance_x;			/* Unit stance x coordinate				*/
	BYTE			stance_y;			/* Unit stance y coordinate				*/
	USHORT			loader;				/* Unit loader record ID (if loaded)			*/
	BYTE			load_cap;			/* Unit load capacity					*/
	BYTE			load_cost;			/* Unit load cost					*/
	BYTE			contact;			/* Unit contact status					*/
	BYTE			rof;				/* Unit rate of fire					*/
	BYTE			tgt;				/* Unit targetting rating				*/
	BYTE			rf;				/* Unit rangefinder rating				*/
	BYTE			fc;				/* Unit firecontrol rating				*/
	BYTE			iv;				/* Unit infrared vision rating				*/
	BYTE			swim;				/* Unit swim capability					*/
	BYTE			men;				/* Unit manpower					*/
	BYTE			men_ori;			/* Unit original manpower				*/
	BYTE			speed;				/* Unit speed						*/
	BYTE			moves;				/* Unit available moves					*/
	USHORT			damage;				/* Unit damage						*/
	USHORT			movdir;				/* Unit movement direction (degrees, CW, 0=east)	*/
	USHORT			shtdir;				/* Unit shooting direction (degrees, CW, 0=east)	*/
	USHORT			target;				/* Current target unit record ID			*/
	BYTE			UTGidx;				/* Unit type grouping index				*/
	USHORT			SPECIAL_OU;			/* Special unknown value				*/
	USHORT			SPECIAL[5];			/* Special unknown values				*/
	SPWAW_UD		UD;				/* Unit UD						*/
} SPWAW_SNAP_OOB_UELRAW;

/* SPWAW OOB: unit raw data */
typedef struct s_SPWAW_SNAP_OOB_URAW {
	USHORT			cnt;				/* raw data count					*/
	SPWAW_SNAP_OOB_UELRAW	*raw;				/* array of raw data elements				*/
	SPWAW_RINDEX		*ridx;				/* record index array					*/
	USHORT			start;				/* unit ID start number					*/
} SPWAW_SNAP_OOB_URAW;

/* SPWAW: raw leader element */
typedef struct s_SPWAW_SNAP_OOB_LELRAW {
	USHORT			RID;				/* Leader record ID					*/
	USHORT			URID;				/* Leader unit record ID				*/
	char			*name;				/* Leader name						*/
	BYTE			rank;				/* Leader rank						*/
	BYTE			ral;				/* Leader rally skill					*/
	BYTE			inf;				/* Leader infantry skill				*/
	BYTE			art;				/* Leader artillery skill				*/
	BYTE			arm;				/* Leader armor skill					*/
	USHORT			kills;				/* Leader kill count					*/
	BYTE			status;				/* Leader status					*/
	SPWAW_UD		UD;				/* Leader UD						*/
} SPWAW_SNAP_OOB_LELRAW;

/* SPWAW OOB: leader raw data */
typedef struct s_SPWAW_SNAP_OOB_LRAW {
	USHORT			cnt;				/* raw data count					*/
	SPWAW_SNAP_OOB_LELRAW	*raw;				/* array of raw data elements				*/
	SPWAW_RINDEX		*ridx;				/* record index array					*/
} SPWAW_SNAP_OOB_LRAW;

/* SPWAW: raw unit position element */
typedef struct s_SPWAW_SNAP_OOB_PELRAW {
	USHORT			RID;				/* Position record ID					*/
	USHORT			URID;				/* Position unit record ID				*/
	USHORT			x;				/* Position x-coord					*/
	USHORT			y;				/* Position y-coord					*/
	BYTE			seen;				/* Unit spotted flag					*/
	SPWAW_UD		UD;				/* Position UD						*/
} SPWAW_SNAP_OOB_PELRAW;

/* SPWAW OOB: position raw data */
typedef struct s_SPWAW_SNAP_OOB_PRAW {
	USHORT			cnt;				/* raw data count					*/
	SPWAW_SNAP_OOB_PELRAW	*raw;				/* array of raw data elements				*/
	SPWAW_RINDEX		*ridx;				/* record index array					*/
} SPWAW_SNAP_OOB_PRAW;

/* SPWAW OOB raw data */
typedef struct s_SPWAW_SNAP_OOB_RAW {
	SPWAW_SNAP_OOB_FRAW	formations;			/* Raw formation data					*/
	SPWAW_SNAP_OOB_URAW	units;				/* Raw unit data					*/
	SPWAW_SNAP_OOB_LRAW	leaders;			/* Raw leader data					*/
	SPWAW_SNAP_OOB_PRAW	positions;			/* Raw position data					*/
} SPWAW_SNAP_OOB_RAW;

/* SPWAW game snapshot: raw data */
typedef struct s_SPWAW_SNAP_RAW {
	SPWAW_SNAP_GAME_RAW	game;				/* Raw game info					*/
	SPWAW_SNAP_OOB_RAW	OOBp1;				/* Raw player1 OOB info					*/
	SPWAW_SNAP_OOB_RAW	OOBp2;				/* Raw player2 OOB info					*/
} SPWAW_SNAP_RAW;



/***************************************/
/***   INTERPRETED DATA STRUCTURES   ***/
/***************************************/

/* SPWAW comment info: interpreted data */
typedef struct s_SPWAW_SNAP_CMT {
	char			*title;				/* Savegame title					*/
}SPWAW_SNAP_CMT;

/* SPWAW battle info: interpreted victory hex data */
typedef struct s_SPWAW_SNAP_VHEX {
	int			x;				/* Victory hex x-coord					*/
	int			y;				/* Victory hex y-coord					*/
	int			value;				/* Victory hex point value				*/
	SPWAW_VHSTATUS		status;				/* Victory hex ownership status				*/
} SPWAW_SNAP_VHEX;

/* SPWAW battle info: interpreted data */
typedef struct s_SPWAW_SNAP_BATTLE_DATA {
	SPWAW_DATE		start;				/* Battle start date/time				*/
	SPWAW_DATE		date;				/* Battle turn date/time				*/
	char			*location;			/* Battle location name					*/
	SPWAW_TERRAIN		terrain;			/* Battle terrain					*/
	SPWAW_WEATHER		weather;			/* Battle weather					*/
	int			visibility;			/* Battle visibility					*/
	int			turn;				/* Battle turn						*/
	int			turn_max;			/* Battle max turn					*/
	SPWAW_BTSTATUS		status;				/* Battle turn status					*/
	BYTE			OOB_p1;				/* Player1 OOB ID					*/
	BYTE			OOB_p2;				/* Player2 OOB ID					*/
	BYTE			OOB_p3;				/* Player3 OOB ID					*/
	BYTE			OOB_p4;				/* Player4 OOB ID					*/
	SPWAW_MISSION		miss_p1;			/* Mission type player1					*/
	SPWAW_MISSION		miss_p2;			/* Mission type player2					*/
	bool			meeting;			/* Meeting engagement flag				*/
	int			credit;				/* Purchase credit points				*/
	SPWAW_SNAP_VHEX		vhex[SPWAW_VHEXCNT];		/* Victory hexes					*/
} SPWAW_SNAP_BATTLE_DATA;

/* SPWAW battle info: strings */
typedef struct s_SPWAW_SNAP_BATTLE_STRINGS {
	char			date[SPWAW_AZSDATE];		/* Battle date string					*/
	char			*terrain;			/* Battle terrain string				*/
	char			*weather;			/* Battle weather string				*/
	char			*status;			/* Battle turn status					*/
	char			*nation_p1;			/* Player1 OOB nation string				*/
	char			*people_p1;			/* Player1 OOB name string				*/
	char			*prefix_p1;			/* Player1 OOB prefix string				*/
	char			*flagid_p1;			/* Player1 OOB flag ID string				*/
	char			*nation_p2;			/* Player2 OOB nation string				*/
	char			*people_p2;			/* Player2 OOB name string				*/
	char			*prefix_p2;			/* Player2 OOB prefix string				*/
	char			*flagid_p2;			/* Player2 OOB flag ID string				*/
	char			*nation_p3;			/* Player3 OOB nation string				*/
	char			*people_p3;			/* Player3 OOB name string				*/
	char			*prefix_p3;			/* Player3 OOB prefix string				*/
	char			*flagid_p3;			/* Player3 OOB flag ID string				*/
	char			*nation_p4;			/* Player4 OOB nation string				*/
	char			*people_p4;			/* Player4 OOB name string				*/
	char			*prefix_p4;			/* Player4 OOB prefix string				*/
	char			*flagid_p4;			/* Player4 OOB flag ID string				*/
	char			*miss_p1;			/* Mission type player1 string				*/
	char			*miss_p2;			/* Mission type player2 string				*/
} SPWAW_SNAP_BATTLE_STRINGS;

/* SPWAW battle info: victory hex statistics */
typedef struct s_SPWAW_SNAP_VHEX_STATS {
	int			count;				/* Victory hex count					*/
	int			value;				/* Total victory hex value				*/
} SPWAW_SNAP_VHEX_STATS;

/* SPWAW battle info: statistics */
typedef struct s_SPWAW_SNAP_BATTLE_STATS {
	SPWAW_SNAP_VHEX_STATS	vhex_stats[SPWAW_VHSTATUS_CNT];	/* Victory hex statistics				*/
} SPWAW_SNAP_BATTLE_STATS;

/* SPWAW battle info */
typedef struct s_SPWAW_SNAP_BATTLE {
	SPWAW_SNAP_BATTLE_DATA		data;			/* Interpreted battle data				*/
	SPWAW_SNAP_BATTLE_STRINGS	strings;		/* Battle info strings					*/
	SPWAW_SNAP_BATTLE_STATS		stats;			/* Battle statistics					*/
} SPWAW_SNAP_BATTLE;

/* SPWAW campaign info: losses */
typedef struct s_SPWAW_SNAP_LOSSES {
	int			men;				/* men losses						*/
	int			art;				/* ART losses						*/
	int			soft;				/* soft vehicles losses					*/
	int			apc;				/* APC losses						*/
	int			afv;				/* ARM losses						*/
	int			gliders;			/* glider losses					*/
	int			air;				/* aircraft losses					*/
} SPWAW_SNAP_LOSSES;

/* SPWAW campaign info: interpreted data */
typedef struct s_SPWAW_SNAP_CAMPAIGN_DATA {
	bool			campaign;			/* Campaign mode active					*/
	SPWAW_DATE		start;				/* Campaign start date					*/
	SPWAW_DATE		end;				/* Campaign end date					*/
	USHORT			battles;			/* Battles fought					*/
	USHORT			majvics;			/* Major victories					*/
	USHORT			minvics;			/* Minor victories					*/
	USHORT			losses;				/* Losses						*/
	USHORT			battles_max;			/* Max battles						*/
	SPWAW_SNAP_LOSSES	P1BL;				/* Player1 battle losses				*/
	SPWAW_SNAP_LOSSES	P2BL;				/* Player2 battle losses				*/
	SPWAW_SNAP_LOSSES	P1TL;				/* Player1 total losses					*/
	SPWAW_SNAP_LOSSES	P2TL;				/* Player2 total losses					*/
	int			P1score;			/* Player1 score					*/
	int			P2score;			/* Player2 score					*/
	SPWAW_BRESULT		P1result;			/* Player1 battle result				*/
	SPWAW_BRESULT		P2result;			/* Player2 battle result				*/
} SPWAW_SNAP_CAMPAIGN_DATA;

/* SPWAW campaign info: strings */
typedef struct s_SPWAW_SNAP_CAMPAIGN_STRINGS {
	char			start[SPWAW_AZSDATE];		/* Campaign start date string				*/
	char			end[SPWAW_AZSDATE];		/* Campaign end date string				*/
	char			*P1result;			/* Player1 battle result				*/
	char			*P2result;			/* Player2 battle result				*/
} SPWAW_SNAP_CAMPAIGN_STRINGS;

/* SPWAW campaign info */
typedef struct s_SPWAW_SNAP_CAMPAIGN {
	SPWAW_SNAP_CAMPAIGN_DATA	data;			/* Interpreted campaign data				*/
	SPWAW_SNAP_CAMPAIGN_STRINGS	strings;		/* Campaign info strings				*/
} SPWAW_SNAP_CAMPAIGN;

/* SPWAW map info: hex data */
typedef struct s_SPWAW_SNAP_MAP_DATA {
	int			h;				/* height						*/
	bool			water;				/* hex has stream/swamp/water				*/
	bool			bridge;				/* hex has bridge					*/
	bool			road;				/* hex has road/railroad/path				*/
	int			conn_road1;			/* primary road connections for hex			*/
	int			conn_road2;			/* secondary road connections for hex			*/
	int			conn_rail;			/* railroad connections for hex				*/
	int			conn_tram;			/* tramline connections for hex				*/
} SPWAW_SNAP_MAP_DATA;

/* SPWAW map info */
typedef struct s_SPWAW_SNAP_MAP {
	int			width;				/* Battle map width					*/
	int			height;				/* Battle map height					*/
	SPWAW_SNAP_MAP_DATA	*data;				/* Battle map hex data					*/
} SPWAW_SNAP_MAP;

/* SPWAW game info */
typedef struct s_SPWAW_SNAP_GAME {
	SPWAW_SNAP_CMT		cmt;				/* Comment info						*/
	SPWAW_SNAP_BATTLE	battle;				/* Battle info						*/
	SPWAW_SNAP_CAMPAIGN	campaign;			/* Campaign info					*/
	SPWAW_SNAP_MAP		map;				/* Map info						*/
} SPWAW_SNAP_GAME;

/* SPWAW coupled unit/formation integer value/id data structure */
typedef struct s_SPWAW_IVID {
	DWORD			v;
	USHORT			i;
} SPWAW_IVID;

/* SPWAW coupled unit/formation float value/id data structure */
typedef struct s_SPWAW_FVID {
	double			v;
	USHORT			i;
} SPWAW_FVID;

/* SPWAW max/min/average/spread (DWORD) data stucture */
typedef struct s_SPWAW_IMMAS {
	SPWAW_IVID		max;				/* Maximum value					*/
	SPWAW_IVID		min;				/* Minimum value					*/
	double			avg;				/* Average value					*/
	DWORD			spr;				/* Spread value						*/
	DWORD			tot;				/* Total summed value					*/
	DWORD			cnt;				/* Total value count					*/
} SPWAW_IMMAS;

/* SPWAW total and split IMMAS set */
typedef struct s_SPWAW_IMMAS_SET {
	SPWAW_IMMAS		all;
	SPWAW_IMMAS		rnk[SPWAW_RLASTCODE+1];
	SPWAW_IMMAS		exp[SPWAW_ELASTCODE+1];
	SPWAW_IMMAS		cls[SPWOOB_UCLASS_LIMIT+1];
} SPWAW_IMMAS_SET;

/* SPWAW max/min/average/spread (double) data stucture */
typedef struct s_SPWAW_FMMAS {
	SPWAW_FVID		max;				/* Maximum value					*/
	SPWAW_FVID		min;				/* Minimum value					*/
	double			avg;				/* Average value					*/
	double			spr;				/* Spread value						*/
	double			tot;				/* Total summed value					*/
	DWORD			cnt;				/* Total value count					*/
} SPWAW_FMMAS;

/* SPWAW total and split FMMAS set */
typedef struct s_SPWAW_FMMAS_SET {
	SPWAW_FMMAS		all;
	SPWAW_FMMAS		rnk[SPWAW_RLASTCODE+1];
	SPWAW_FMMAS		exp[SPWAW_ELASTCODE+1];
	SPWAW_FMMAS		cls[SPWOOB_UCLASS_LIMIT+1];
} SPWAW_FMMAS_SET;

/* SPWAW calculated general OOB attributes */
typedef struct s_SPWAW_SNAP_OOB_ATTR {
	double			ready;				/* Readiness						*/
	DWORD			kills;				/* Recorded kills					*/
	DWORD			losses;				/* Recorded losses					*/
} SPWAW_SNAP_OOB_ATTR;

/* SPWAW calculated OOB unit attributes */
typedef struct s_SPWAW_SNAP_OOB_UATTR {
	SPWAW_SNAP_OOB_ATTR	gen;				/* general attributes					*/
} SPWAW_SNAP_OOB_UATTR;

/* SPWAW calculated OOB formation attributes */
typedef struct s_SPWAW_SNAP_OOB_FATTR {
	SPWAW_SNAP_OOB_ATTR	gen;				/* general attributes					*/
	SPWAW_IMMAS		mmas_exp;
	SPWAW_IMMAS		mmas_mor;
	SPWAW_IMMAS		mmas_sup;
	SPWAW_IMMAS		mmas_ral;
	SPWAW_IMMAS		mmas_inf;
	SPWAW_IMMAS		mmas_arm;
	SPWAW_IMMAS		mmas_art;
	SPWAW_FMMAS		mmas_ready;
	SPWAW_IMMAS		mmas_kills;
} SPWAW_SNAP_OOB_FATTR;

/* SPWAW calculated OOB force attributes */
typedef struct s_SPWAW_SNAP_OOB_GATTR {
	SPWAW_SNAP_OOB_ATTR	gen;				/* general attributes					*/
	SPWAW_IMMAS_SET		mmas_uexp;
	SPWAW_IMMAS_SET		mmas_umor;
	SPWAW_IMMAS_SET		mmas_usup;
	SPWAW_IMMAS_SET		mmas_ural;
	SPWAW_IMMAS_SET		mmas_uinf;
	SPWAW_IMMAS_SET		mmas_uarm;
	SPWAW_IMMAS_SET		mmas_uart;
	SPWAW_FMMAS_SET		mmas_uready;
	SPWAW_IMMAS_SET		mmas_ukills;
	SPWAW_IMMAS_SET		mmas_ucost;
	SPWAW_IMMAS_SET		mmas_uspeed;
	SPWAW_FMMAS		mmas_fready;
	SPWAW_IMMAS		mmas_fkills;
	SPWAW_IMMAS		mmas_flosses;
} SPWAW_SNAP_OOB_GATTR;

/* Forward declarations for convenience */
typedef struct s_SPWAW_SNAP_OOB_FEL		SPWAW_SNAP_OOB_FEL;
typedef struct s_SPWAW_SNAP_OOB_UEL		SPWAW_SNAP_OOB_UEL;
typedef struct s_SPWAW_SNAP_OOB_TREE_FEL	SPWAW_SNAP_OOB_TREE_FEL;

/* Record index/element pointer union type */
typedef union u_SPWAW_SNAP_OOB_PTR {
	USHORT			rid;				/* record index						*/
	SPWAW_SNAP_OOB_UEL	*up;				/* unit pointer						*/
	SPWAW_SNAP_OOB_FEL	*fp;				/* formation pointer					*/
} SPWAW_SNAP_OOB_PTR;

/* SPWAW formation element info: interpreted data */
typedef struct s_SPWAW_SNAP_OOB_FEL_DATA {
	USHORT			idx;
	USHORT			RID;				/* Formation record ID						*/
	USHORT			FID;				/* Formation ID number						*/
	USHORT			OOBrid;				/* Formation OOB record ID					*/
	SPWAW_FSTATUS		status;				/* Formation campaign status					*/
	SPWOOB_FSTAT		fstatus;			/* Formation force status					*/
	SPWOOB_FTYPE		type;				/* Formation type						*/
	SPWAW_SNAP_OOB_PTR	leader;				/* Formation leader unit record ID or pointer			*/
	SPWAW_SNAP_OOB_PTR	hcmd;				/* Formation higher command leader unit record ID or pointer	*/
	USHORT			ucnt;				/* Unit count							*/
	USHORT			ucnt_core;			/* Unit count (core units)					*/
	USHORT			ucnt_support;			/* Unit count (support units)					*/
	SPWAW_SNAP_OOB_UEL	**ulist;			/* Unit pointer list						*/
} SPWAW_SNAP_OOB_FEL_DATA;

/* SPWAW formation element info: strings */
typedef struct s_SPWAW_SNAP_OOB_FEL_STRINGS {
	char			name[16];			/* Formation ID name					*/
	char			*status;			/* Formation campaign status string			*/
	char			*fstatus;			/* Formation force status string			*/
	char			*type;				/* Formation type string				*/
	char			*utype;				/* Formation unit type					*/
} SPWAW_SNAP_OOB_FEL_STRINGS;

/* SPWAW formation element info */
struct s_SPWAW_SNAP_OOB_FEL {
	BYTE				status;			/* flag indicating formation core/support status	*/
	SPWAW_SNAP_OOB_FEL_DATA		data;
	SPWAW_SNAP_OOB_FEL_STRINGS	strings;
	SPWAW_SNAP_OOB_FATTR		attr;
};

/* SPWAW formation list */
typedef struct s_SPWAW_SNAP_OOB_F {
	USHORT			cnt;
	SPWAW_SNAP_OOB_FEL	*list;
} SPWAW_SNAP_OOB_F;

/* SPWAW unit element info: data */
typedef struct s_SPWAW_SNAP_OOB_UEL_DATA {
	SPWAW_UNIT_TYPE		dutype;				/* Detected unit type					*/
	USHORT			idx;
	USHORT			uidx;
	USHORT			ulidx;
	USHORT			elidx;
	USHORT			RID;				/* Unit record ID	*/
	int			FRID;				/* Formation record ID	*/
	int			FMID;				/* Formation major ID	*/
	int			FSID;				/* Formation sub ID	*/
	SPWAW_SNAP_OOB_FEL	*formation;
	BYTE			OOB;				/* Unit OOB country ID					*/
	USHORT			OOBrid;				/* Unit OOB record ID					*/
	char			*type;
	char			*name;
	SPWAW_RANK		rank;
	SPWAW_RANK		brank;				/* battle rank, initially SPWAW_RKIA, updated when added to dossier	*/
	int			kills;
	SPWAW_USTATUS		status;
	SPWAW_ENTR		entr;
	bool			alive;
	//bool			lost;
	bool			spotted;
	int			hcnt;
	int			hcnt_left;
	int			cost;
	int			damage;
	bool			crew;				/* Unit has (or had) a bailed out crew			*/
	SPWAW_ABAND		aband;				/* Abandonment status					*/
	SPWAW_SNAP_OOB_PTR	aunit;				/* Abandoned or left unit				*/
	bool			loaded;				/* Loaded status					*/
	SPWAW_SNAP_OOB_PTR	loader;				/* Loader unit						*/
	int			exp;
	SPWAW_EXP		eclass;
	int			mor;
	int			sup;
	int			ral;
	int			inf;
	int			art;
	int			arm;
	int			speed;
	int			smkdev;
	SPWAW_CSTATUS		contact;
	int			rof;
	int			tgt;
	int			rf;
	int			fc;
	int			iv;
	int			swim;
	int			UTGidx;
	SPWOOB_UTYPE		utype;
	SPWOOB_UCLASS		uclass;
	int			posx;				/* Unit grid X position, -1 if dead			*/
	int			posy;				/* Unit grid Y position, -1 if dead			*/
} SPWAW_SNAP_OOB_UEL_DATA;

/* SPWAW unit element info: strings */
typedef struct s_SPWAW_SNAP_OOB_UEL_STRINGS {
	char			uid[16];
	char			*people;
	char			*rank;
	char			*exp;
	char			*status;
	char			*entr;
	char			*contact;
	char			*utype;
	char			*uclass;
} SPWAW_SNAP_OOB_UEL_STRINGS;

/* SPWAW unit element info */
struct s_SPWAW_SNAP_OOB_UEL {
	bool				core;			/* flag indicating core unit				*/
	SPWAW_SNAP_OOB_UEL_DATA		data;
	SPWAW_SNAP_OOB_UEL_STRINGS	strings;
	SPWAW_SNAP_OOB_UATTR		attr;
};

/* SPWAW unit list */
typedef struct s_SPWAW_SNAP_OOB_U {
	USHORT			cnt;
	SPWAW_SNAP_OOB_UEL	*list;
} SPWAW_SNAP_OOB_U;

/* SPWAW force formation statistics */
typedef struct s_SPWAW_SNAP_OOB_FSTATS {
	USHORT			cnt;				/* Total formations count				*/
	USHORT			cnt_left;			/* Total current formations count			*/
} SPWAW_SNAP_OOB_FSTATS;

/* SPWAW force unit statistics */
typedef struct s_SPWAW_SNAP_OOB_USTATS {
	USHORT			cnt;				/* Total units count				*/
	USHORT			cnt_left;			/* Total current units count			*/
	USHORT			rnk_cnt[SPWAW_RLASTCODE+1];	/* Units rank histogram				*/
	USHORT			exp_cnt[SPWAW_ELASTCODE+1];	/* Units experience histogram			*/
	USHORT			cls_cnt[SPWOOB_UCLASS_LIMIT+1];	/* Units class histogram			*/
	DWORD			rnk_loss[SPWAW_RLASTCODE+1];	/* Losses rank histogram			*/
	DWORD			exp_loss[SPWAW_ELASTCODE+1];	/* Losses experience histogram			*/
	DWORD			cls_loss[SPWOOB_UCLASS_LIMIT+1];/* Losses class histogram			*/
} SPWAW_SNAP_OOB_USTATS;

/* SPWAW battle statistics */
typedef struct s_SPWAW_SNAP_OOB_STATS {
	SPWAW_SNAP_OOB_FSTATS	fstats;				/* Formation statistics					*/
	SPWAW_SNAP_OOB_USTATS	ustats;				/* Unit statistics					*/
	USHORT			hcnt;				/* Total head count at start				*/
	USHORT			hcnt_left;			/* Total current head count				*/
} SPWAW_SNAP_OOB_STATS;

/* SPWAW force information */
typedef struct s_SPWAW_SNAP_OOB_FORCE {
	SPWAW_SNAP_OOB_F	formations;			/* Formation list					*/
	SPWAW_SNAP_OOB_U	units;				/* Unit list						*/
	SPWAW_SNAP_OOB_U	crews;				/* Crew list						*/
	SPWAW_SNAP_OOB_UEL	*leader;			/* Leader						*/
	SPWAW_SNAP_OOB_GATTR	attr;				/* Attributes						*/
	SPWAW_SNAP_OOB_STATS	stats;				/* Statistics						*/
} SPWAW_SNAP_OOB_FORCE;

/* SPWAW OOB info */
typedef struct s_SPWAW_SNAP_OOB {
	int			side;				/* side ID (0=player1, 1=player2)			*/
	BYTE			OOB;				/* OOB ID number					*/
	char			*nation;			/* nationality name string				*/
	char			*people;			/* nationality possessive string			*/
	SPWAW_SNAP_OOB_FORCE	core;				/* Core force information				*/
	SPWAW_SNAP_OOB_FORCE	support;			/* Support force information				*/
	SPWAW_SNAP_OOB_FORCE	battle;				/* Complete battle force information			*/
} SPWAW_SNAP_OOB;



/*******************************/
/***   MAIN DATA STRUCTURE   ***/
/*******************************/

/* SPWAW game snapshot source information */
typedef struct s_SPWAW_SNAP_SOURCE {
	char			*path;				/* Source game file path				*/
	char			*file;				/* Source game file name				*/
	FILETIME		date;				/* Source game file modification date			*/
} SPWAW_SNAP_SOURCE;

/* SPWAW game snapshot */
typedef struct s_SPWAW_SNAPSHOT {
	SPWAW_SNAP_SOURCE	src;				/* Snapshot game source info				*/
	SPWAW_SNAP_RAW		raw;				/* Raw snapshot data					*/
	SPWAW_SNAP_GAME		game;				/* Game info						*/
	SPWAW_SNAP_OOB		OOBp1;				/* Player1 OOB info					*/
	SPWAW_SNAP_OOB		OOBp2;				/* Player2 OOB info					*/
	SPWOOB			*oobdat;			/* OOB data						*/
	bool			freeoobdat;			/* indicates if OOB data must be freed			*/
	void			*stab;				/* \internal string table				*/
	SPWAW_BATTLE_TYPE	type;				/* Snapshot battle type					*/
} SPWAW_SNAPSHOT;

/* SPWAW game snapshot file info */
typedef struct s_SPWAW_SNAPSHOT_INFO {
	char			title[SPWAW_AZSCMTTITLE+1];	/* Snapshot title					*/
	SPWAW_DATE		start;				/* Snapshot battle start date				*/
	BYTE			turn;				/* Snapshot battle turn					*/
	SPWAW_DATE		date;				/* Snapshot battle turn date				*/
	char			stamp[32];			/* snapshot battle date/turn string			*/
	char			location[SPWAW_AZSLOCATION+1];	/* Snapshot battle location				*/
	char			filename[MAX_PATH+1];		/* Snapshot source game filename			*/
	FILETIME		filedate;			/* Snapshot source game filedate			*/
	SPWAW_BATTLE_TYPE	type;				/* Snapshot battle type					*/
} SPWAW_SNAPSHOT_INFO;



/* Invalid index value */
#define	SPWAW_BADIDX	((USHORT)0xFFFF)



/*** API ***/

extern SPWAWLIB_API SPWAW_ERROR	SPWAW_snap_make	(const char *dir, int id, SPWAW_SNAPSHOT **snap);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_snap_info	(const char *file, SPWAW_SNAPSHOT_INFO *info);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_snap_load	(const char *file, SPWAW_SNAPSHOT **snap);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_snap_save	(SPWAW_SNAPSHOT **snap, const char *file, bool compress);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_snap_free (SPWAW_SNAPSHOT **snap);
extern SPWAWLIB_API SPWAW_ERROR	SPWAW_snap_copy (SPWAW_SNAPSHOT **src, SPWAW_SNAPSHOT **dst);

#endif		/* SPWAW_LIB_SNAPSHOT_H */

/** @} */
