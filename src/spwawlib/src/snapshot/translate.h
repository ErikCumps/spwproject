/** \file
 * The SPWaW Library - snapshot handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	TRANSLATE_H
#define	TRANSLATE_H	1

#include <spwawlib_types.h>

/* --- terrain translation --- */
extern SPWAW_TERRAIN		raw2terrain	(BYTE id);
extern char *			terrain2str	(SPWAW_TERRAIN id);

/* --- weather translation --- */
extern SPWAW_WEATHER		raw2weather	(BYTE id);
extern char *			weather2str	(SPWAW_TERRAIN tid, SPWAW_WEATHER wid);

/* --- mission translation --- */
extern SPWAW_MISSION		raw2mission	(BYTE id);
extern char *			mission2str	(SPWAW_MISSION id);

/* --- formation status translation --- */
extern SPWAW_FSTATUS		raw2fstatus	(BYTE id);
extern char *			fstatus2str	(SPWAW_FSTATUS id);

/* --- unit status translation --- */
extern SPWAW_USTATUS		raw2ustatus	(BYTE id);
extern char *			ustatus2str	(SPWAW_USTATUS id);

/* --- contact status translation --- */
extern SPWAW_CSTATUS		raw2cstatus	(BYTE id);
extern char *			cstatus2str	(SPWAW_CSTATUS id);

/* --- entrenchment translation --- */
extern SPWAW_ENTR		raw2entr	(BYTE id);
extern char *			entr2str	(SPWAW_ENTR id);

/* --- rank translation --- */
extern SPWAW_RANK		raw2rank	(BYTE id);
extern BYTE			rank2raw	(SPWAW_RANK rank);
extern char *			rank2str	(SPWAW_RANK id);

/* --- experience translation --- */
extern SPWAW_EXP		raw2exp		(BYTE exp);
extern char *			exp2str		(SPWAW_EXP exp);

/* --- victory hex status translation --- */
extern SPWAW_VHSTATUS		raw2vhstatus	(BYTE id);
extern char *			vhstatus2str	(SPWAW_VHSTATUS id);

/* --- battle result translation --- */
extern SPWAW_BRESULT		raw2bresult	(SHORT result);
extern char *			bresult2str	(SPWAW_BRESULT result);

/* --- battle status translation --- */
extern char *			btstatus2str	(SPWAW_BTSTATUS id);

/* --- formation and unit ID translation --- */
extern void			FID2str		(BYTE id, char *buf, int len);
extern char *			FID2str		(BYTE id);
extern void			UID2str		(BYTE id, BYTE subid, char *buf, int len);
extern char *			UID2str		(BYTE id, BYTE subid);

/* --- abandonment status translation --- */
extern SPWAW_ABAND		raw2aband	(BYTE aband);
extern BYTE			aband2raw	(SPWAW_ABAND aband);
extern char *			aband2str	(SPWAW_ABAND id);

/* --- unit type translation --- */
extern SPWAW_UNIT_TYPE		raw2unittype	(BYTE type);
extern BYTE			unittype2raw	(SPWAW_UNIT_TYPE type);

#endif	/* TRANSLATE_H */
