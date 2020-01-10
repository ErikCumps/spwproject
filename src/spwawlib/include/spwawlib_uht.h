/** \file
 * The SPWaW Library - unit history tracking API.
 *
 * Copyright (C) 2019-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 *
 * \addtogroup API
 * @{
 */

#ifndef	SPWAW_LIB_UHT_H
#define	SPWAW_LIB_UHT_H	1

#include <spwawlib_api.h>
#include <spwawlib_types.h>

/* Forward declarations for convenience */
typedef struct s_SPWAW_DOSSIER		SPWAW_DOSSIER;
typedef struct s_SPWAW_DOSSIER_UIR	SPWAW_DOSSIER_UIR;
typedef struct s_SPWAW_BATTLE		SPWAW_BATTLE;
typedef struct s_SPWAW_UHTE		SPWAW_UHTE;
typedef struct s_SPWAW_UHT		SPWAW_UHT;

/* --- Unit history flags --- */

/* Unit has no status */
#define	UHT_NOSTATUS	0x0000

/* Unit was renamed after previous battle */
#define	UHT_RENAMED	0x0001

/* Unit was killed and replaced after previous battle */
#define	UHT_REPLACED	0x0002

/* Unit was reassigned or cross-attached after previous battle */
#define	UHT_REASSIGNED	0x0004

/* Unit was upgraded after previous battle */
#define	UHT_UPGRADED	0x0008

/* Unit was promoted after previous battle */
#define	UHT_PROMOTED	0x0010

/* Unit was demoted after previous battle */
#define	UHT_DEMOTED	0x0020

/* Unit was promoted or demoted after previous battle */
#define	UHT_RERANKED	(UHT_PROMOTED|UHT_DEMOTED)

/* Unit was damaged during this battle */
#define	UHT_DAMAGED	0x0040

/* Unit was abandoned during this battle */
#define	UHT_ABANDONED	0x0080

/* Unit was destroyed during this battle */
#define	UHT_DESTROYED	0x0100

/* SPWAW unit history tracking element */
typedef struct s_SPWAW_UHTE {
	SPWAW_UHT		*uht;			/* Pointer to parent UHT structure					*/
	unsigned int		idx;			/* Element index							*/
	char			*lname;			/* Unit identity: name							*/
	char			*UID;			/* Unit identity: UID							*/
	char			*uname;			/* Unit identity: equipment						*/
	SPWAW_RANK		rank;			/* Unit identity: rank							*/
	int			FMID;			/* Unit identity: formation major ID					*/
	int			FSID;			/* Unit identity: formation sub ID					*/
	SPWAW_BATTLE_DATE	FBI;			/* Battle date for the battle the unit was first seen in		*/
	USHORT			FUI;			/* Unit index for the battle the unit was first seen in			*/
	SPWAW_BATTLE_DATE	LBI;			/* Battle date for the battle the unit was last seen in			*/
	USHORT			status;			/* Unit history status							*/
	SPWAW_UHTE		*prev;			/* Pointer to previous element in unit history chain			*/
	SPWAW_UHTE		*next;			/* Pointer to next element in unit history chain			*/
							/* Status specific values:						*/
	int			v_damage;		/* UHT_DAMAGED: damage							*/
} SPWAW_UHTE;

/* SPWAW unit history tracking battle info data */
typedef struct s_SPWAW_UHT_BINFO {
	SPWAW_BATTLE_DATE	bdate;			/* Battle date								*/
	USHORT			cnt;			/* Number of UHT element pointers in the list				*/
	SPWAW_UHTE		**list;			/* UHT element pointers list						*/
} SPWAW_UHT_BINFO;

/* SPWAW unit history tracking battle info list */
typedef struct s_SPWAW_UHT_BLIST {
	unsigned int		cnt;			/* Number of used elements in the UHT_BINFO list			*/
	SPWAW_UHT_BINFO		**info;			/* UHT_BINFO list							*/
	unsigned int		len;			/* Total length (in elements) of the UHT_BINFO list			*/
} SPWAW_UHT_BLIST;

/* SPWAW unit history tracking */
typedef struct s_SPWAW_UHT {
	unsigned int		cnt;			/* Number of used elements in the UHTE list				*/
	unsigned int		icnt;			/* Number of initial elements in the UHTE list				*/
	SPWAW_UHTE		**list;			/* UHTE list								*/
	SPWAW_UHTE		**smap;			/* UHTE sort map							*/
	unsigned int		len;			/* Total length (in elements) of the UHTE list				*/
	SPWAW_DOSSIER		*dossier;		/* Pointer to parent dossier						*/
	SPWAW_UHT_BLIST		blist;			/* Battle info list							*/
} SPWAW_UHT;

/*** API ***/

/* Returns the UIR (if found) for the specified base UHTE and battle date */
extern SPWAWLIB_API SPWAW_DOSSIER_UIR *	SPWAW_UHT_lookup		(SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate);

/* Checks if the specified UHTE is the initial UHTE of a UHTE chain */
static inline bool
SPWAW_UHT_is_initial (SPWAW_UHTE *uhte) { return (uhte && !uhte->prev); }

/* Checks if the unit of specified UHTE was active at the specified battle date */
extern SPWAWLIB_API bool		SPWAW_UHT_is_active		(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate);

/* Checks if the unit of specified UHTE was commissioned at the specified battle date */
extern SPWAWLIB_API bool		SPWAW_UHT_is_commissioned	(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate);

/* Checks if the unit of specified UHTE was decommissioned during the campaign */
extern SPWAWLIB_API bool		SPWAW_UHT_is_decommissioned	(SPWAW_UHTE *uhte);

/* Checks if the unit of specified UHTE was decommissioned at the specified battle date */
extern SPWAWLIB_API bool		SPWAW_UHT_is_decommissioned	(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate);

/* Checks if the unit of specified UHTE has the specified status at the specified battle date */
extern SPWAWLIB_API bool		SPWAW_UHT_is			(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate, USHORT status);

/* Checks if the unit of specified UHTE has the specified status during the campaign */
extern SPWAWLIB_API bool		SPWAW_UHT_is			(SPWAW_UHTE *uhte, USHORT status);

/* Returns the initial UHTE of the chain containing the specified UHTE */
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_first			(SPWAW_UHTE *uhte);

/* Returns the first UHTE (with the specified status) of the chain containing the specified UHTE */
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_first			(SPWAW_UHTE *uhte, USHORT status);

/* Returns the final UHTE of the chain containing the specified UHTE */
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_last			(SPWAW_UHTE *uhte);

/* Returns the last UHTE (with the specified status) of the chain containing the specified UHTE */
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_last			(SPWAW_UHTE *uhte, USHORT status);

/* Returns the next UHTE (with the specified status) in the chain containing the specified UHTE */
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_next			(SPWAW_UHTE *uhte, USHORT status);

/* Returns the previous UHTE (with the specified status) in the chain containing the specified UHTE */
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_prev			(SPWAW_UHTE *uhte, USHORT status);

/* --- Convenience macros: status check (for specific battle)  --- */
#define	SPWAW_UHT_is_renamed(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_RENAMED)
#define	SPWAW_UHT_is_replaced(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_REPLACED)
#define	SPWAW_UHT_is_reassigned(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_REASSIGNED)
#define	SPWAW_UHT_is_upgraded(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_UPGRADED)
#define	SPWAW_UHT_is_promoted(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_PROMOTED)
#define	SPWAW_UHT_is_demoted(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_DEMOTED)
#define	SPWAW_UHT_is_reranked(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_RERANKED)
#define	SPWAW_UHT_is_damaged(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_DAMAGED)
#define	SPWAW_UHT_is_abandoned(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_ABANDONED)
#define	SPWAW_UHT_is_destroyed(u_,bd_)		SPWAW_UHT_is(u_, bd_, UHT_DESTROYED)

/* --- Convenience macros: status check (for entire campaign) --- */
#define	SPWAW_UHT_has_rename(u_)		SPWAW_UHT_is(u_, UHT_RENAMED)
#define	SPWAW_UHT_has_replacement(u_)		SPWAW_UHT_is(u_, UHT_REPLACED)
#define	SPWAW_UHT_has_reassignment(u_)		SPWAW_UHT_is(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_has_upgrade(u_)		SPWAW_UHT_is(u_, UHT_UPGRADED)
#define	SPWAW_UHT_has_promotion(u_)		SPWAW_UHT_is(u_, UHT_PROMOTED)
#define	SPWAW_UHT_has_demotion(u_)		SPWAW_UHT_is(u_, UHT_DEMOTED)
#define	SPWAW_UHT_has_rerank(u_)		SPWAW_UHT_is(u_, UHT_RERANKED)
#define	SPWAW_UHT_has_damaged(u_)		SPWAW_UHT_is(u_, UHT_DAMAGED)
#define	SPWAW_UHT_has_abandoned(u_)		SPWAW_UHT_is(u_, UHT_ABANDONED)
#define	SPWAW_UHT_has_destroyed(u_)		SPWAW_UHT_is(u_, UHT_DESTROYED)

/* --- Convenience macros: first status access (for entire campaign) --- */
#define	SPWAW_UHT_first_rename(u_)		SPWAW_UHT_first(u_, UHT_RENAMED)
#define	SPWAW_UHT_first_replacement(u_)		SPWAW_UHT_first(u_, UHT_REPLACED)
#define	SPWAW_UHT_first_reassignment(u_)	SPWAW_UHT_first(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_first_upgrade(u_)		SPWAW_UHT_first(u_, UHT_UPGRADED)
#define	SPWAW_UHT_first_promotion(u_)		SPWAW_UHT_first(u_, UHT_PROMOTED)
#define	SPWAW_UHT_first_demotion(u_)		SPWAW_UHT_first(u_, UHT_DEMOTED)
#define	SPWAW_UHT_first_rerank(u_)		SPWAW_UHT_first(u_, UHT_RERANKED)
#define	SPWAW_UHT_first_damaged(u_)		SPWAW_UHT_first(u_, UHT_DAMAGED)
#define	SPWAW_UHT_first_abandoned(u_)		SPWAW_UHT_first(u_, UHT_ABANDONED)
#define	SPWAW_UHT_first_destroyed(u_)		SPWAW_UHT_first(u_, UHT_DESTROYED)

/* --- Convenience macros: next status access (for entire campaign) --- */
#define	SPWAW_UHT_next_rename(u_)		SPWAW_UHT_next(u_, UHT_RENAMED)
#define	SPWAW_UHT_next_replacement(u_)		SPWAW_UHT_next(u_, UHT_REPLACED)
#define	SPWAW_UHT_next_reassignment(u_)		SPWAW_UHT_next(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_next_upgrade(u_)		SPWAW_UHT_next(u_, UHT_UPGRADED)
#define	SPWAW_UHT_next_promotion(u_)		SPWAW_UHT_next(u_, UHT_PROMOTED)
#define	SPWAW_UHT_next_demotion(u_)		SPWAW_UHT_next(u_, UHT_DEMOTED)
#define	SPWAW_UHT_next_rerank(u_)		SPWAW_UHT_next(u_, UHT_RERANKED)
#define	SPWAW_UHT_next_damaged(u_)		SPWAW_UHT_next(u_, UHT_DAMAGED)
#define	SPWAW_UHT_next_abandoned(u_)		SPWAW_UHT_next(u_, UHT_ABANDONED)
#define	SPWAW_UHT_next_destroyed(u_)		SPWAW_UHT_next(u_, UHT_DESTROYED)

/* --- Convenience macros: last status access (for entire campaign) --- */
#define	SPWAW_UHT_last_rename(u_)		SPWAW_UHT_last(u_, UHT_RENAMED)
#define	SPWAW_UHT_last_replacement(u_)		SPWAW_UHT_last(u_, UHT_REPLACED)
#define	SPWAW_UHT_last_reassignment(u_)		SPWAW_UHT_last(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_last_upgrade(u_)		SPWAW_UHT_last(u_, UHT_UPGRADED)
#define	SPWAW_UHT_last_promotion(u_)		SPWAW_UHT_last(u_, UHT_PROMOTED)
#define	SPWAW_UHT_last_demotion(u_)		SPWAW_UHT_last(u_, UHT_DEMOTED)
#define	SPWAW_UHT_last_rerank(u_)		SPWAW_UHT_last(u_, UHT_RERANKED)
#define	SPWAW_UHT_last_damaged(u_)		SPWAW_UHT_last(u_, UHT_DAMAGED)
#define	SPWAW_UHT_last_abandoned(u_)		SPWAW_UHT_last(u_, UHT_ABANDONED)
#define	SPWAW_UHT_last_destroyed(u_)		SPWAW_UHT_last(u_, UHT_DESTROYED)

/* --- Convenience macros: previous status access (for entire campaign) --- */
#define	SPWAW_UHT_prev_rename(u_)		SPWAW_UHT_prev(u_, UHT_RENAMED)
#define	SPWAW_UHT_prev_replacement(u_)		SPWAW_UHT_prev(u_, UHT_REPLACED)
#define	SPWAW_UHT_prev_reassignment(u_)		SPWAW_UHT_prev(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_prev_upgrade(u_)		SPWAW_UHT_prev(u_, UHT_UPGRADED)
#define	SPWAW_UHT_prev_promotion(u_)		SPWAW_UHT_prev(u_, UHT_PROMOTED)
#define	SPWAW_UHT_prev_demotion(u_)		SPWAW_UHT_prev(u_, UHT_DEMOTED)
#define	SPWAW_UHT_prev_rerank(u_)		SPWAW_UHT_prev(u_, UHT_RERANKED)
#define	SPWAW_UHT_prev_damaged(u_)		SPWAW_UHT_prev(u_, UHT_DAMAGED)
#define	SPWAW_UHT_prev_abandoned(u_)		SPWAW_UHT_prev(u_, UHT_ABANDONED)
#define	SPWAW_UHT_prev_destroyed(u_)		SPWAW_UHT_prev(u_, UHT_DESTROYED)

#endif	/* SPWAW_LIB_UHT_H */

/** @} */
