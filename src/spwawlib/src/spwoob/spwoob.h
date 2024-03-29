/** \file
 * The SPWaW Library - SPWaW OOB handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	INTERNAL_SPWOOB_H
#define	INTERNAL_SPWOOB_H	1

#include <spwawlib_spwoob.h>

extern SPWAW_ERROR	SPWOOB_new		(SPWOOB **oob);
extern SPWAW_ERROR	SPWOOB_copy		(SPWOOB *dst, SPWOOB *src);
extern void		SPWOOB_free		(SPWOOB **oob);

extern SPWAW_ERROR	SPWOOB_load		(SPWOOB *oob, SPWAW_GAME_TYPE gametype, const char *dir);
extern SPWAW_ERROR	SPWOOB_load		(SPWOOB *oob, int fd);
extern SPWAW_ERROR	SPWOOB_save		(SPWOOB *oob, int fd, bool compress);

extern SPWAW_ERROR	SPWOOB_compare		(SPWOOB *oob1, SPWOOB *oob2);

extern void		spwoob_free		(SPWOOB *oob);

extern SPWOOB_DATA *	spwoob_data		(SPWOOB *oob, BYTE id);

extern const char *	spwoob_id2people	(SPWAW_GAME_TYPE gametype, BYTE id);
extern const char *	spwoob_id2people	(SPWAW_GAME_TYPE gametype, BYTE id, int year, int month);

extern const char *	spwoob_id2nation	(SPWAW_GAME_TYPE gametype, BYTE id);
extern const char *	spwoob_id2nation	(SPWAW_GAME_TYPE gametype, BYTE id, int year, int month);

extern const char *	spwoob_id2prefix	(SPWAW_GAME_TYPE gametype, BYTE id);
extern const char *	spwoob_id2prefix	(SPWAW_GAME_TYPE gametype, BYTE id, int year, int month);

extern const char *	spwoob_id2flagid	(SPWAW_GAME_TYPE gametype, BYTE id);
extern const char *	spwoob_id2flagid	(SPWAW_GAME_TYPE gametype, BYTE id, int year, int month);

extern void		spwoob_dump		(SPWOOB *oob, char *base, bool raw);

#endif	/* INTERNAL_SPWOOB_H */
