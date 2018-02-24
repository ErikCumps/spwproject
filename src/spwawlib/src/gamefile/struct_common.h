/** \file
 * The SPWaW Library - gamefile handling.
 *
 * Copyright (C) 2007-2017 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_STRUCT_COMMON_H
#define	INTERNAL_STRUCT_COMMON_H	1

#include "common.h"
#include "common/sizes.h"
#include "strtab/strtab.h"

/* Common savegame map icon data */
typedef struct s_MAP_ICON {
	BYTE	file;	/* File ID (TerIIIzJ.shp)	*/
	BYTE	icon;	/* Icon ID in SHP file		*/
} MAP_ICON;

/* forward declarations */
typedef struct s_UEL UEL;
typedef struct s_FEL FEL;



/* Supported unit types */
typedef enum e_UTYPE {
	UTYPE_UNIT = 0,		/* Unit				*/
	UTYPE_CREW,		/* Crew				*/
	UTYPE_SPAU		/* Special Attached Unit	*/
} UTYPE;

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
		USHORT		FSID;				/* Formation sub-ID				*/
		BYTE		OOB;				/* OOB country ID				*/
		BYTE		OOBrid;				/* OOB record ID				*/
		SPWOOB_UTYPE	OOBtype;			/* OOB unit type				*/
		FEL		*formation;			/* Associated formation				*/
		UTYPE		type;				/* Unit type: unit/crew/spau			*/
		USHORT		LRID;				/* Loader unit record ID			*/
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
		BYTE		player;				/* Player ID					*/
		USHORT		leader;				/* Leader unit ID				*/
		BYTE		OOBrid;				/* Formation OOB record ID			*/
		char		name[SPWAW_AZSNAME];		/* Formation name				*/
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

#endif	/* INTERNAL_STRUCT_COMMON_H */
