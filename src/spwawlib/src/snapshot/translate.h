#ifndef	TRANSLATE_H
#define	TRANSLATE_H	1

#include <spwawlib_types.h>

extern SPWAW_TERRAIN		raw2terrain	(BYTE id);
extern SPWAW_WEATHER		raw2weather	(BYTE id);
extern SPWAW_MISSION		raw2mission	(BYTE id);
extern SPWAW_FSTATUS		raw2fstatus	(BYTE id);
extern SPWAW_USTATUS		raw2ustatus	(BYTE id);
extern SPWAW_ENTR		raw2entr	(BYTE id);
extern SPWAW_RANK		raw2rank	(BYTE id);
extern SPWAW_EXP		raw2exp		(BYTE exp);
extern SPWAW_VHSTATUS		raw2vhstatus	(BYTE id);
extern bool			rawtfs2water	(BYTE tfs1, BYTE tfs2, BYTE tfs3, BYTE tfs4);
extern bool			rawtfs2bridge	(BYTE tfs1, BYTE tfs2, BYTE tfs3, BYTE tfs4);
extern bool			rawtfs2road	(BYTE tfs1, BYTE tfs2, BYTE tfs3, BYTE tfs4);

extern char *			terrain2str	(SPWAW_TERRAIN id);
extern char *			weather2str	(SPWAW_TERRAIN tid, SPWAW_WEATHER wid);
extern char *			mission2str	(SPWAW_MISSION id);
extern char *			fstatus2str	(SPWAW_FSTATUS id);
extern char *			ustatus2str	(SPWAW_USTATUS id);
extern char *			entr2str	(SPWAW_ENTR id);
extern char *			rank2str	(SPWAW_RANK id);
extern char *			btstatus2str	(SPWAW_BTSTATUS id);
extern char *			vhstatus2str	(SPWAW_VHSTATUS id);

extern void			FID2str		(BYTE id, char *buf, int len);
extern char *			FID2str		(BYTE id);
extern void			UID2str		(BYTE id, BYTE subid, char *buf, int len);
extern char *			UID2str		(BYTE id, BYTE subid);
extern char *			exp2str		(SPWAW_EXP id);
extern char *			aband2str	(SPWAW_ABAND id);

#endif	/* TRANSLATE_H */
