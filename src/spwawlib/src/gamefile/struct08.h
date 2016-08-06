#ifndef	INTERNAL_STRUCT08_H
#define	INTERNAL_STRUCT08_H	1

#include "gamefile/struct_common.h"

/* (...sides of the hex are number 1,2,4,8,16,32 clockwise around from the east and summed up) */
typedef struct s_CONNMAP {
	BYTE	EE:1;					/* East		*/
	BYTE	SE:1;					/* Southeast	*/
	BYTE	SW:1;					/* Southwest	*/
	BYTE	WW:1;					/* West		*/
	BYTE	NW:1;					/* Northwest	*/
	BYTE	NE:1;					/* Northeast	*/
	BYTE	reserved:2;
} CONNMAP;

typedef struct s_MAP_TERRAIN {
	union {
		struct {
			BYTE	has_field:1;		/* Terrain contains field			*/
			BYTE	has_slope:1;		/* Terrain contains slope			*/
			BYTE	has_trees:1;		/* Terrain contains trees			*/
			BYTE	has_stream:1;		/* Terrain contains stream			*/
			BYTE	has_bld_stone:1;	/* Terrain contains stone building		*/
			BYTE	has_bld_wood:1;		/* Terrain contains wooden building		*/
			BYTE	has_road2:1;		/* Terrain contains secondary road		*/
			BYTE	has_road1:1;		/* Terrain contains primary road		*/
		} bits;
		BYTE		has_T1;			/* Terrain features from set #1			*/
	} uT1;
	union {
		struct {
			BYTE	has_brg_wood:1;		/* Terrain contains wooden bridge		*/
			BYTE	has_brg_stone:1;	/* Terrain contains stone bridge		*/
			BYTE	has_swamp:1;		/* Terrain contains swamp			*/
			BYTE	has_water:1;		/* Terrain contains water			*/
			BYTE	has_rough:1;		/* Terrain contains rough			*/
			BYTE	has_T2F5:1;		/* Terrain contains feature 0x20 from set #2	*/
			BYTE	has_T2F6:1;		/* Terrain contains feature 0x40 from set #2	*/
			BYTE	has_T2F7:1;		/* Terrain contains feature 0x80 from set #2	*/
		} bits;
		BYTE		has_T2;			/* Terrain features from set #2			*/
	} uT2;
	union {
		struct {
			BYTE	has_railroad:1;		/* Terrain contains railroad			*/
			BYTE	has_T3F1:1;		/* Terrain contains feature 0x02 from set #3	*/
			BYTE	has_T3F2:1;		/* Terrain contains feature 0x04 from set #3	*/
			BYTE	has_T3F3:1;		/* Terrain contains feature 0x08 from set #3	*/
			BYTE	has_shwater:1;		/* Terrain contains shallow water		*/
			BYTE	has_dpwater:1;		/* Terrain contains deep water			*/
			BYTE	has_T3F6:1;		/* Terrain contains feature 0x40 from set #3	*/
			BYTE	has_orchard:1;		/* Terrain contains orchard/vineryard		*/
		} bits;
		BYTE		has_T3;			/* Terrain features from set #3			*/
	} uT3;
	union {
		struct {
			BYTE	has_wall:1;		/* Terrain contains wall/hedge			*/
			BYTE	has_path:1;		/* Terrain contains path			*/
			BYTE	has_bocage:1;		/* Terrain contains bocage			*/
			BYTE	has_T4F3:1;		/* Terrain contains feature 0x08 from set #4	*/
			BYTE	has_T4F4:1;		/* Terrain contains feature 0x10 from set #4	*/
			BYTE	has_T4F5:1;		/* Terrain contains feature 0x20 from set #4	*/
			BYTE	has_T4F6:1;		/* Terrain contains feature 0x40 from set #4	*/
			BYTE	has_T4F7:1;		/* Terrain contains feature 0x80 from set #4	*/
		} bits;
		BYTE		has_T4;			/* Terrain features from set #4			*/
	} uT4;
	SHORT	height;					/* Height					*/
	char	__data00[6];
	union {
		CONNMAP	cmap_road2;
		BYTE	conn_road2;			/* Secondary road connections			*/
	} uR2;
	union {
		CONNMAP	cmap_road1;
		BYTE	conn_road1;			/* Primary road connections			*/
	} uR1;
	union {
		CONNMAP	cmap_rail;
                BYTE	conn_rail;			/* Railroad connections				*/
	} uRR;
	char	__data01[5];
} MAP_TERRAIN;

typedef struct s_STRUCT08 {
	union {
		char	raw[SIZESEC08];
		struct {
			MAP_TERRAIN	terrain[MAPWIDTH][MAPHEIGHT];
		} d;
	} u;
} STRUCT08;

extern SPWAW_ERROR	sec08_save_snapshot (GAMEDATA *src, SPWAW_SNAPSHOT *dst, STRTAB *stab);

#endif	/* INTERNAL_STRUCT08_H */
