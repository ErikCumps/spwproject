/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INDEX_H
#define	INDEX_H	1

#include <spwawlib_api.h>
#include <spwawlib_snapshot.h>

extern SPWAW_ERROR	build_fridx	(SPWAW_SNAP_OOB_FRAW *src);
extern SPWAW_ERROR	build_uridx	(SPWAW_SNAP_OOB_URAW *src);
extern SPWAW_ERROR	build_lridx	(SPWAW_SNAP_OOB_LRAW *src);
extern SPWAW_ERROR	build_pridx	(SPWAW_SNAP_OOB_PRAW *src);

extern USHORT		fridx		(SPWAW_SNAP_OOB_FRAW *src, USHORT cri);
extern USHORT		uridx		(SPWAW_SNAP_OOB_URAW *src, USHORT cri);
extern USHORT		lridx		(SPWAW_SNAP_OOB_LRAW *src, USHORT cri);
extern USHORT		pridx		(SPWAW_SNAP_OOB_PRAW *src, USHORT cri);

#endif	/* INDEX_H */
