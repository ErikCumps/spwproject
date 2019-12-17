/** \file
 * The SPWaW Library - unit history tracking handling.
 *
 * Copyright (C) 2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UHT_H
#define	UHT_H	1

#include <spwawlib_uht.h>
#include <spwawlib_dossier.h>

#define	UHT_LISTCNT_GROW	128

extern SPWAW_ERROR	UHT_init	(SPWAW_UHT *uht, SPWAW_DOSSIER *dossier);
extern SPWAW_ERROR	UHT_clean	(SPWAW_UHT *uht);
extern SPWAW_ERROR	UHT_clear	(SPWAW_UHT *uht);
extern SPWAW_ERROR	UHT_load	(int fd, SPWAW_UHT *dst);
extern SPWAW_ERROR	UHT_save	(SPWAW_UHT *src, int fd, bool compress);

extern SPWAW_ERROR	UHT_update	(SPWAW_UHT *uht, SPWAW_BATTLE *b);
extern SPWAW_ERROR	UHT_rebuild	(SPWAW_UHT *uht);

extern SPWAW_ERROR	UHT_debug_dump	(SPWAW_UHT *uht);

#endif	/* UHT_H */
