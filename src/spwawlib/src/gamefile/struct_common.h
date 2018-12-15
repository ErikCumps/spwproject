/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT_COMMON_H
#define	INTERNAL_STRUCT_COMMON_H	1

#include "common.h"
#include "common/sizes.h"
#include "strtab/strtab.h"

/* Common savegame map icon data */
//typedef struct s_MAP_TILE {
//	BYTE	file;	/* File ID (TerIIIzJ.shp)	*/
//	BYTE	icon;	/* Icon ID in SHP file		*/
//} MAP_TILE;

/* Common savegame map tile data */
typedef struct s_MAP_TILE {
	USHORT	FileTileId;	/* FileID * 250 + TileId	*/
} MAP_TILE;


/* forward declarations */
typedef struct s_UEL UEL;
typedef struct s_FEL FEL;


/* Intermediate unit element - used during formation/unit detection */
typedef struct s_UEL {
	struct s_list {
		UEL	*prev;					/* Unit list: previous unit			*/
		UEL	*next;					/* Unit list: next unit				*/
	}	l;
	struct s_data {
		USHORT		RID;				/* Unit record ID				*/
		char		name[SPWAW_AZSNAME];		/* Unit name					*/
		USHORT		FRID;				/* Formation record ID				*/
		USHORT		FMID;				/* Formation ID					*/
		BYTE		FSID;				/* Formation sub-ID				*/
		BYTE		OOB;				/* OOB country ID				*/
		USHORT		OOBrid;				/* OOB record ID				*/
		SPWOOB_UTYPE	OOBtype;			/* OOB unit type				*/
		FEL		*formation;			/* Associated formation				*/
		SPWAW_UNIT_TYPE	type;				/* Unit type: unit/crew/spau			*/
		USHORT		LRID;				/* Detected leader unit record ID		*/
		SPWAW_ABAND	aband;				/* Unit abandonment status			*/
		USHORT		loader;				/* Loader unit record ID			*/
		bool		vrfloader;			/* Verified loader				*/
		bool		needvrfldrtst;			/* Requires loader verification acceptance test	*/
		union u_link {
			UEL	*crew;				/* Crew unit (if unit has a crew)		*/
			UEL	*parent;			/* Parent unit (if unit is a crew)		*/
		}		link;
	}	d;
} UEL;

/* Intermediate unit list - used during formation/unit detection */
typedef	struct s_ULIST {
	struct s_storage {
		UEL	list[UNITCOUNT];			/* Unit data array				*/
		USHORT	nidx;					/* Next available index in the array		*/
	}	s;
	UEL	*head;						/* Pointer to the start of the list		*/
	USHORT	cnt;						/* Number of units in the list			*/
} ULIST;

void	init_ULIST	(ULIST &ul);
void	dump_ULIST	(ULIST &ul);
void	dump_UEL	(UEL *uel, char *prefix);
UEL *	reserve_UEL	(ULIST &ul);
bool	commit_UEL	(ULIST &ul, UEL *uel);
void	drop_UEL	(ULIST &ul, UEL *uel);
UEL *	lookup_ULIST	(ULIST &ul, USHORT urid);



/* Intermediate formation element - used during formation/unit detection */
typedef struct s_FEL {
	struct s_list {
		FEL	*prev;					/* Formation list: previous formation		*/
		FEL	*next;					/* Formation list: next formation		*/
	}	l;
	struct s_data {
		USHORT		RID;				/* Formation record ID				*/
		USHORT		rawFID;				/* Raw formation ID				*/
		USHORT		player;				/* Player ID					*/
		USHORT		leader;				/* Leader unit ID				*/
		USHORT		OOBrid;				/* Formation OOB record ID			*/
		char		name[SPWAW_AZSNAME];		/* Formation name				*/
		BYTE		status;				/* Formation status				*/
		USHORT		FID;				/* Adjusted formation ID			*/
		BYTE		OOB;				/* OOB country ID				*/
		BYTE		unit_cnt;			/* Number of associated units			*/
		UEL		*unit_lst[MAXFORMATIONUNITS];	/* List of associated units			*/
	}	d;
} FEL;

/* Intermediate formation list - used during formation/unit detection */
typedef	struct s_FLIST {
	struct s_storage {
		FEL	list[FORMCOUNT];			/* Formation data array				*/
		USHORT	nidx;					/* Next available index in the array		*/
	}	s;
	FEL	*head;						/* Pointer to the start of the list		*/
	USHORT	cnt;						/* Number of formations in the list		*/
} FLIST;

void	init_FLIST	(FLIST &fl);
void	dump_FLIST	(FLIST &fl);
void	dump_FEL	(FEL *fel, char *prefix);
FEL *	reserve_FEL	(FLIST &fl);
bool	commit_FEL	(FLIST &fl, FEL *fel);
void	drop_FEL	(FLIST &fl, FEL *fel);
FEL *	lookup_FLIST	(FLIST &fl, USHORT frid);



bool	add_crew_to_unit	(UEL *cel, UEL *uel);
bool	add_unit_to_formation	(FEL *fel, UEL *uel);


/* Intermediate combined formation/unit list - used during formation/unit detection */
typedef struct s_FULIST {
	FLIST	fl;						/* Formation list				*/
	ULIST	ul;						/* Unit list					*/
} FULIST;

void	init_FULIST	(FULIST &l);
void	dump_FULIST	(FULIST &l);


typedef struct s_STRUCT52 {
	union u_u {
		char	raw[SIZESEC52];
		struct s_d {
			char	data[SIZESEC52];
		} d;
	} u;
} STRUCT52;
typedef struct s_STRUCT53 {
	union u_u {
		char	raw[SIZESEC53];
		struct s_d {
			char	data[SIZESEC53];
		} d;
	} u;
} STRUCT53;
typedef struct s_MAPHEXNAME {
	char	name[64];
} MAPHEXNAME;
typedef struct s_STRUCT54 {
	union u_u {
		char	raw[SIZESEC54];
		struct s_d {
			MAPHEXNAME	name[100];
		} d;
	} u;
} STRUCT54;
typedef struct s_STRUCT56 {
	union u_u {
		char	raw[SIZESEC56];
		struct s_d {
			char	data[SIZESEC56];
		} d;
	} u;
} STRUCT56;
typedef struct s_STRUCT57 {
	union u_u {
		char	raw[SIZESEC57];
		struct s_d {
			char	data[SIZESEC57];
		} d;
	} u;
} STRUCT57;
typedef struct s_STRUCT59 {
	union u_u {
		char	raw[SIZESEC59];
		struct s_d {
			char	data[SIZESEC59];
		} d;
	} u;
} STRUCT59;

#endif	/* INTERNAL_STRUCT_COMMON_H */
