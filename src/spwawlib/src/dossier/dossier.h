/** \file
 * The SPWaW Library - dossier handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
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
extern SPWAW_ERROR	dossier_add			(SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap, SPWAW_BTURN **bturn);
extern SPWAW_ERROR	dossier_del			(SPWAW_DOSSIER *ptr, SPWAW_BTURN *bturn);
extern SPWAW_ERROR	dossier_del			(SPWAW_DOSSIER *ptr, SPWAW_BATTLE *battle);
extern SPWAW_ERROR	dossier_savelist		(SPWAW_DOSSIER *dossier, SPWAW_SAVELIST **list);
extern SPWAW_ERROR	dossier_snaplist		(SPWAW_DOSSIER *dossier, SPWAW_SNAPLIST **list);

extern SPWAW_BATTLE *	dossier_find_battle		(SPWAW_DOSSIER *ptr, SPWAW_SNAPSHOT *snap);
extern SPWAW_BTURN *	dossier_find_bturn		(SPWAW_BATTLE *ptr, SPWAW_SNAPSHOT *snap);
extern bool		battle_contains_bturn		(SPWAW_BATTLE *ptr, SPWAW_BTURN *bturn);
extern SPWAW_ERROR	dossier_fix_battle_info		(SPWAW_BTURN *bt, SPWAW_BTURN *ref);
extern SPWAW_ERROR	dossier_prep_bturn_info		(SPWAW_BTURN *bt);
extern void		dossier_free_bturn_info		(SPWAW_DOSSIER_BIRS *ip);
extern SPWAW_ERROR	dossier_update_battle_info	(SPWAW_BATTLE *ptr);
extern SPWAW_ERROR	dossier_update_battle_rainfo	(SPWAW_BATTLE *src, SPWAW_BATTLE *dst);

extern SPWAW_ERROR	dossier_export			(int src, FILE *dst);
extern SPWAW_ERROR	dossier_import			(FILE *src, int dst);

#endif	/* DOSSIER_H */