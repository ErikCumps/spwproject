/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2018 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SNAPSHOT_H
#define	SNAPSHOT_H	1

#include <spwawlib_api.h>
#include "strtab/strtab.h"
#include "gamefile/gamefile.h"
#include "snapshot/snapfile.h"

extern SPWAW_ERROR		load_from_game		(GAMEDATA *src, SPWAW_SNAPSHOT *dst);

extern SPWAW_ERROR		snapnew			(SPWAW_SNAPSHOT **dst, SPWOOB *oobptr, STRTAB *stabptr);
extern SPWAW_ERROR		snapsave		(SPWAW_SNAPSHOT *src, int fd, bool do_oob, bool do_stab, bool compress);
extern SPWAW_ERROR		snaploadinfo		(int fd, SPWAW_SNAPSHOT_INFO *info);
extern SPWAW_ERROR		snaploadhdrs		(int fd, SNAP_HEADER *mhdr, SNAP_SOURCE *shdr, SNAP_INFO *ihdr, SNAP_OOBHDR *ohdr=NULL);
extern SPWAW_ERROR		snapload		(int fd, SPWAW_SNAPSHOT *dst, STRTAB *stabptr);
extern SPWAW_ERROR		snapclean		(SPWAW_SNAPSHOT *ptr, STRTAB *stabptr);
extern SPWAW_ERROR		snapcopy		(SPWAW_SNAPSHOT *src, SPWAW_SNAPSHOT *dst);
extern SPWAW_ERROR		snapint			(SPWAW_SNAPSHOT *ptr);
extern SPWAW_ERROR		snapstabmerge		(SPWAW_SNAPSHOT *snap, STRTAB *dst);
extern SPWAW_ERROR		snapstabremove		(SPWAW_SNAPSHOT *snap, STRTAB *dst);
extern SPWAW_ERROR		snapbattlefix		(SPWAW_SNAPSHOT *ptr, SPWAW_SNAPSHOT *ref);

extern SPWAW_ERROR		OOB_link		(SPWAW_SNAP_OOB *oob, bool prepsf);

extern bool			check_unitid		(DWORD urid, SPWAW_SNAP_OOB_URAW *ptr, SPWAW_SNAP_OOB_UELRAW **udata);
extern bool			check_formationid	(DWORD frid, SPWAW_SNAP_OOB_FRAW *ptr, BYTE *fid, SPWAW_SNAP_OOB_FELRAW **fdata);

extern SPWAW_SNAP_OOB_UEL *	unitbyid		(DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr);
extern SPWAW_SNAP_OOB_UEL *	crewbyid		(DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr);
extern SPWAW_SNAP_OOB_UEL *	unitorcrewbyid		(DWORD urid, SPWAW_SNAP_OOB_FORCE *ptr);
extern SPWAW_SNAP_OOB_FEL *	formationbyid		(DWORD frid, SPWAW_SNAP_OOB_FORCE *ptr);

extern SPWAW_ERROR		snapexport		(int src, FILE *dst);
extern SPWAW_ERROR		snapimport		(FILE *src, int dst);

extern void			snapint_addmmas		(SPWAW_IMMAS *dst, DWORD val, USHORT id);
extern void			snapint_addmmas		(SPWAW_FMMAS *dst, double val, USHORT id);

extern void			snapint_addmmas_set	(SPWAW_IMMAS_SET *dst, DWORD val, SPWAW_SNAP_OOB_UEL *ptr);
extern void			snapint_addmmas_set	(SPWAW_FMMAS_SET *dst, double val, SPWAW_SNAP_OOB_UEL *ptr);

#endif	/* SNAPSHOT_H */
