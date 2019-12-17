/** \file
 * The SPWaW Library - unit history tracking API.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
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
typedef struct s_SPWAW_UHTE		SPWAW_UHTE;
typedef struct s_SPWAW_UHT		SPWAW_UHT;

/* --- Unit history flags --- */

/* Unit has no status */
#define	UHT_NOSTATUS	0x0000

/* Unit was renamed */
#define	UHT_RENAMED	0x0001

/* Unit was killed and replaced */
#define	UHT_REPLACED	0x0002

/* Unit was reassigned or cross-attached */
#define	UHT_REASSIGNED	0x0004

/* Unit was upgraded */
#define	UHT_UPGRADED	0x0008

/* Unit was promoted */
#define	UHT_PROMOTED	0x0010

/* Unit was demoted */
#define	UHT_DEMOTED	0x0020

/* Unit was promoted or demoted */
#define	UHT_RERANKED	(UHT_PROMOTED|UHT_DEMOTED)

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
	SPWAW_UHTE		*prev;			/* Pointer to previous element in unit history list			*/
	SPWAW_UHTE		*next;			/* Pointer to next element in unit history list				*/
} SPWAW_UHTE;

/* SPWAW unit history tracking structure */
typedef struct s_SPWAW_UHT {
	unsigned int		cnt;			/* Number of used UHT elements in the list				*/
	SPWAW_UHTE		**list;			/* Pointer to UHT element list						*/
	SPWAW_UHTE		**smap;			/* Pointer to UHT element sort map					*/
	unsigned int		len;			/* Lentgh (in elements) of the UHT elements list			*/
	SPWAW_DOSSIER		*dossier;		/* Pointer to parent dossier						*/
} SPWAW_UHT;

/*** API ***/
extern SPWAWLIB_API SPWAW_DOSSIER_UIR *	SPWAW_UHT_lookup		(SPWAW_UHTE *base, SPWAW_BATTLE_DATE *bdate);

static inline bool
SPWAW_UHT_is_initial (SPWAW_UHTE *uhte) {
	return (uhte && !uhte->prev);
}

extern SPWAWLIB_API bool		SPWAW_UHT_is_active		(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate);
extern SPWAWLIB_API bool		SPWAW_UHT_is_commissioned	(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate);
extern SPWAWLIB_API bool		SPWAW_UHT_is_decommissioned	(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate);

extern SPWAWLIB_API bool		SPWAW_UHT_is			(SPWAW_UHTE *uhte, SPWAW_BATTLE_DATE *bdate, USHORT status);
extern SPWAWLIB_API bool		SPWAW_UHT_is			(SPWAW_UHTE *uhte, USHORT status);

extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_first			(SPWAW_UHTE *uhte);
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_first			(SPWAW_UHTE *uhte, USHORT status);
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_last			(SPWAW_UHTE *uhte);
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_last			(SPWAW_UHTE *uhte, USHORT status);
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_next			(SPWAW_UHTE *uhte, USHORT status);
extern SPWAWLIB_API SPWAW_UHTE *	SPWAW_UHT_prev			(SPWAW_UHTE *uhte, USHORT status);

#define	SPWAW_UHT_is_renamed(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_RENAMED)
#define	SPWAW_UHT_is_replaced(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_REPLACED)
#define	SPWAW_UHT_is_reassigned(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_REASSIGNED)
#define	SPWAW_UHT_is_upgraded(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_UPGRADED)
#define	SPWAW_UHT_is_promoted(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_PROMOTED)
#define	SPWAW_UHT_is_demoted(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_DEMOTED)
#define	SPWAW_UHT_is_reranked(u_,b_)		SPWAW_UHT_is(u_, bd_, UHT_RERANKED)

#define	SPWAW_UHT_has_rename(u_)		SPWAW_UHT_is(u_, UHT_RENAMED)
#define	SPWAW_UHT_has_replacement(u_)		SPWAW_UHT_is(u_, UHT_REPLACED)
#define	SPWAW_UHT_has_reassignment(u_)		SPWAW_UHT_is(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_has_upgrade(u_)		SPWAW_UHT_is(u_, UHT_UPGRADED)
#define	SPWAW_UHT_has_promotion(u_)		SPWAW_UHT_is(u_, UHT_PROMOTED)
#define	SPWAW_UHT_has_demotion(u_)		SPWAW_UHT_is(u_, UHT_DEMOTED)
#define	SPWAW_UHT_has_rerank(u_)		SPWAW_UHT_is(u_, UHT_RERANKED)

#define	SPWAW_UHT_first_rename(u_)		SPWAW_UHT_first(u_, UHT_RENAMED)
#define	SPWAW_UHT_first_replacement(u_)		SPWAW_UHT_first(u_, UHT_REPLACED)
#define	SPWAW_UHT_first_reassignment(u_)	SPWAW_UHT_first(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_first_upgrade(u_)		SPWAW_UHT_first(u_, UHT_UPGRADED)
#define	SPWAW_UHT_first_promotion(u_)		SPWAW_UHT_first(u_, UHT_PROMOTED)
#define	SPWAW_UHT_first_demotion(u_)		SPWAW_UHT_first(u_, UHT_DEMOTED)
#define	SPWAW_UHT_first_rerank(u_)		SPWAW_UHT_first(u_, UHT_RERANKED)

#define	SPWAW_UHT_next_rename(u_)		SPWAW_UHT_next(u_, UHT_RENAMED)
#define	SPWAW_UHT_next_replacement(u_)		SPWAW_UHT_next(u_, UHT_REPLACED)
#define	SPWAW_UHT_next_reassignment(u_)		SPWAW_UHT_next(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_next_upgrade(u_)		SPWAW_UHT_next(u_, UHT_UPGRADED)
#define	SPWAW_UHT_next_promotion(u_)		SPWAW_UHT_next(u_, UHT_PROMOTED)
#define	SPWAW_UHT_next_demotion(u_)		SPWAW_UHT_next(u_, UHT_DEMOTED)
#define	SPWAW_UHT_next_rerank(u_)		SPWAW_UHT_next(u_, UHT_RERANKED)

#define	SPWAW_UHT_last_rename(u_)		SPWAW_UHT_last(u_, UHT_RENAMED)
#define	SPWAW_UHT_last_replacement(u_)		SPWAW_UHT_last(u_, UHT_REPLACED)
#define	SPWAW_UHT_last_reassignment(u_)		SPWAW_UHT_last(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_last_upgrade(u_)		SPWAW_UHT_last(u_, UHT_UPGRADED)
#define	SPWAW_UHT_last_promotion(u_)		SPWAW_UHT_last(u_, UHT_PROMOTED)
#define	SPWAW_UHT_last_demotion(u_)		SPWAW_UHT_last(u_, UHT_DEMOTED)
#define	SPWAW_UHT_last_rerank(u_)		SPWAW_UHT_last(u_, UHT_RERANKED)

#define	SPWAW_UHT_prev_rename(u_)		SPWAW_UHT_prev(u_, UHT_RENAMED)
#define	SPWAW_UHT_prev_replacement(u_)		SPWAW_UHT_prev(u_, UHT_REPLACED)
#define	SPWAW_UHT_prev_reassignment(u_)		SPWAW_UHT_prev(u_, UHT_REASSIGNED)
#define	SPWAW_UHT_prev_upgrade(u_)		SPWAW_UHT_prev(u_, UHT_UPGRADED)
#define	SPWAW_UHT_prev_promotion(u_)		SPWAW_UHT_prev(u_, UHT_PROMOTED)
#define	SPWAW_UHT_prev_demotion(u_)		SPWAW_UHT_prev(u_, UHT_DEMOTED)
#define	SPWAW_UHT_prev_rerank(u_)		SPWAW_UHT_prev(u_, UHT_RERANKED)

#endif	/* SPWAW_LIB_UHT_H */

/** @} */
