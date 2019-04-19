/** \file
 * The SPWaW Library - gamefile handling - intermediate formation/unit element lists.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	FULIST_H
#define	FULIST_H	1

#include <spwawlib_defines.h>
#include <spwawlib_spwoob_types.h>
#include <spwawlib_types.h>
#include "common/sizes.h"

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
		BYTE		FMID;				/* Formation ID					*/
		BYTE		FSID;				/* Formation sub-ID				*/
		BYTE		OOB;				/* OOB country ID				*/
		BYTE		OOBrid;				/* OOB record ID				*/
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
		// FIXME!
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
		BYTE		rawFID;				/* Raw formation ID				*/
		BYTE		player;				/* Player ID					*/
		USHORT		leader;				/* Leader unit ID				*/
		BYTE		OOBrid;				/* Formation OOB record ID			*/
		char		name[SPWAW_AZSNAME];		/* Formation name				*/
		BYTE		status;				/* Formation status				*/
		BYTE		FID;				/* Adjusted formation ID			*/
		BYTE		OOB;				/* OOB country ID				*/
		BYTE		unit_cnt;			/* Number of associated units			*/
		// FIXME?
		UEL		*unit_lst[MAXFORMATIONUNITS];	/* List of associated units			*/
	}	d;
} FEL;

/* Intermediate formation list - used during formation/unit detection */
typedef	struct s_FLIST {
	struct s_storage {
		// FIXME!
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

#endif	/* FULIST_H */
