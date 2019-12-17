/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	DOSSIER_H
#define	DOSSIER_H	1

#include <spwawlib_dossier.h>

extern SPWAW_ERROR	dossier_new			(SPWAW_DOSSIER **dst);
extern SPWAW_ERROR	dossier_save			(SPWAW_DOSSIER *src, int fd, bool compress);
extern SPWAW_ERROR	dossier_loadinfo		(int fd, SPWAW_DOSSIER_INFO *dst);
extern SPWAW_ERROR	dossier_load			(int fd, SPWAW_DOSSIER *dst);
extern SPWAW_ERROR	dossier_clean			(SPWAW_DOSSIER *ptr);
extern void		dossier_clean_battle		(SPWAW_BATTLE *b);
extern void		dossier_clean_turn		(SPWAW_BTURN *t);

extern SPWAW_ERROR	dossier_add_to_campaign		(SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn);
extern SPWAW_ERROR	dossier_add_new_battle		(SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, const char *name, SPWAW_BATTLE **battle);
extern SPWAW_ERROR	dossier_add_to_battle		(SPWAW_BATTLE *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn);
extern SPWAW_ERROR	dossier_del			(SPWAW_DOSSIER *ptr, SPWAW_BTURN *bturn);
extern SPWAW_ERROR	dossier_del			(SPWAW_DOSSIER *ptr, SPWAW_BATTLE *battle);
extern SPWAW_ERROR	dossier_savelist		(SPWAW_DOSSIER *dossier, SPWAW_SAVELIST **list);
extern SPWAW_ERROR	dossier_snaplist		(SPWAW_DOSSIER *dossier, SPWAW_SNAPLIST **list);

extern SPWAW_BATTLE *	dossier_find_battle		(SPWAW_DOSSIER *ptr, SPWAW_BATTLE_DATE *bdate);
extern SPWAW_BATTLE *	dossier_find_battle		(SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap);
extern SPWAW_BTURN *	dossier_find_bturn		(SPWAW_BATTLE *ptr, SPWAW_SNAPSHOT *snap);
extern bool		battle_contains_bturn		(SPWAW_BATTLE *ptr, SPWAW_BTURN *bturn);
extern SPWAW_ERROR	dossier_fix_battle_info		(SPWAW_BTURN *bt, SPWAW_BTURN *ref);
extern SPWAW_ERROR	dossier_prep_bturn_info		(SPWAW_BTURN *bt);
extern void		dossier_free_bturn_info		(SPWAW_DOSSIER_BIRS *ip);
extern SPWAW_ERROR	dossier_update_battle_info	(SPWAW_BATTLE *ptr);
extern SPWAW_ERROR	dossier_update_battle_rainfo	(SPWAW_BATTLE *src, SPWAW_BATTLE *dst);
extern SPWAW_ERROR	dossier_set_battle_props	(SPWAW_BATTLE *battle);

extern SPWAW_ERROR	dossier_update_campaign_props	(SPWAW_DOSSIER *ptr);
extern SPWAW_ERROR	dossier_update_dossier_stats	(SPWAW_DOSSIER *ptr);

typedef struct s_BIRURR {
	SPWAW_BATTLE	*b;
	USHORT		i;
} BIRURR;

typedef struct s_BIRURR_FILTER {
	char		*lname;
	char		*UID;
	char		*uname;
	SPWAW_RANK	*rank;
} BIRURR_FILTER;

#define	INIT_BIRURR_FILTER(f_)	BIRURR_FILTER f_; clear_ptr (&f_)

extern SPWAW_ERROR	dossier_search_back	(BIRURR &fr, BIRURR &rr);
extern SPWAW_ERROR	dossier_search_back	(BIRURR &fr, BIRURR &rr, BIRURR_FILTER &filter);

#endif	/* DOSSIER_H */
