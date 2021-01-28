/** \file
 * The SPWaW Library - winSPMBT gamefile handling.
 *
 * Copyright (C) 2021 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SECTION08_WINSPMBT_H
#define	SECTION08_WINSPMBT_H	1

#include "gamefile/winspmbt/defines_winspmbt.h"

///* (...sides of the hex are number 1,2,4,8,16,32 clockwise around from the east and summed up) */
//typedef struct s_CONNMAP {
//	BYTE	EE:1;					/* East		*/
//	BYTE	SE:1;					/* Southeast	*/
//	BYTE	SW:1;					/* Southwest	*/
//	BYTE	WW:1;					/* West		*/
//	BYTE	NW:1;					/* Northwest	*/
//	BYTE	NE:1;					/* Northeast	*/
//	BYTE	reserved:2;
//} CONNMAP;

typedef struct s_WINSPMBT_TFSBITS1 {
	BYTE	has_field:1;		/* Terrain contains field			*/
	BYTE	has_slope:1;		/* Terrain contains slope			*/
	BYTE	has_trees:1;		/* Terrain contains trees			*/
	BYTE	has_stream:1;		/* Terrain contains stream			*/
	BYTE	has_bld_stone:1;	/* Terrain contains stone building		*/
	BYTE	has_bld_wood:1;		/* Terrain contains wooden building		*/
	BYTE	has_road2:1;		/* Terrain contains secondary road		*/
	BYTE	has_road1:1;		/* Terrain contains primary road		*/
} WINSPMBT_TFSBITS1;

typedef struct s_WINSPMBT_TFSBITS2 {
	BYTE	has_brg_wood:1;		/* Terrain contains wooden bridge		*/
	BYTE	has_brg_stone:1;	/* Terrain contains stone bridge		*/
	BYTE	has_swamp:1;		/* Terrain contains swamp			*/
	BYTE	has_water:1;		/* Terrain contains water			*/
	BYTE	has_rough:1;		/* Terrain contains rough			*/
	BYTE	has_hole:1;		/* Terrain contains hole			*/
	BYTE	has_trench1:1;		/* Terrain contains trench type #1		*/
	BYTE	has_trench2:1;		/* Terrain contains trench type #2		*/
} WINSPMBT_TFSBITS2;

typedef struct s_WINSPMBT_TFSBITS3 {
	BYTE	has_mud:1;		/* Terrain contains mud				*/
	BYTE	has_hedgerow:1;		/* Terrain contains hedgerow			*/
	BYTE	has_snowdrift:1;	/* Terrain contains snow drift			*/
	BYTE	has_softsand:1;		/* Terrain contains soft sand			*/
	BYTE	has_ditch:1;		/* Terrain contains ditch			*/
	BYTE	has_T3F5:1;		/* Terrain contains feature 0x20 from set #3	*/
	BYTE	has_T3F6:1;		/* Terrain contains feature 0x40 from set #3	*/
	BYTE	has_tallgrass:1;	/* Terrain contains tall grass			*/
} WINSPMBT_TFSBITS3;

typedef struct s_WINSPMBT_TFSBITS4 {
	BYTE	has_T4F0:1;		/* Terrain contains feature 0x01 from set #4	*/
	BYTE	has_T4F1:1;		/* Terrain contains feature 0x02 from set #4	*/
	BYTE	has_T4F2:1;		/* Terrain contains feature 0x04 from set #4	*/
	BYTE	has_railroad:1;		/* Terrain contains railroad			*/
	BYTE	has_tramline:1;		/* Terrain contains tramline			*/
	BYTE	has_T4F5:1;		/* Terrain contains feature 0x20 from set #4	*/
	BYTE	has_T4F6:1;		/* Terrain contains feature 0x40 from set #4	*/
	BYTE	has_T4F7:1;		/* Terrain contains feature 0x80 from set #4	*/
} WINSPMBT_TFSBITS4;

typedef struct s_WINSPMBT_MAP_TERRAIN {
	SHORT	height;					/* Height					*/
	BYTE	defense;				/* Terrain defense modifier?			*/
	char	__data00[1];
	BYTE	spotting;				/* Terrain LoS spotting modifier?		*/
	char	__data01[1];
	BYTE	movement;				/* Current unit moving radius?			*/
	union u_uT1 {
		WINSPMBT_TFSBITS1	bits1;
		BYTE			has_T1;		/* Terrain features from set #1			*/
	} uT1;
	union u_uT2 {
		WINSPMBT_TFSBITS2	bits2;
		BYTE			has_T2;		/* Terrain features from set #2			*/
	} uT2;
	union u_uT3 {
		WINSPMBT_TFSBITS3	bits3;
		BYTE			has_T3;		/* Terrain features from set #3			*/
	} uT3;
	union u_uT4 {
		WINSPMBT_TFSBITS4	bits4;
		BYTE			has_T4;		/* Terrain features from set #4			*/
	} uT4;
	char	__data02[2];
	BYTE	smoke;					/* Smoke value					*/
	BYTE	name;					/* Map name index				*/
} WINSPMBT_MAP_TERRAIN;

typedef struct s_WINSPMBT_SECTION08 {
	union u_u {
		char	raw[WINSPMBT_SIZESEC08];
		struct s_d {
			WINSPMBT_MAP_TERRAIN	terrain[WINSPMBT_MAPWIDTH][WINSPMBT_MAPHEIGHT];
		} d;
	} u;
} WINSPMBT_SECTION08;

#endif	/* SECTION08_WINSPMBT_H */
