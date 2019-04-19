/** \file
 * The SPWaW Library - utility code: unknown data handling.
 *
 * Copyright (C) 2007-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	UD_H
#define	UD_H	1

#include <spwawlib_ud.h>

extern void		UD_init		(SPWAW_UD *ud, DWORD size);
extern SPWAW_UD *	UD_new		(DWORD size);
extern void		UD_add		(SPWAW_UD *ud, DWORD addr, DWORD size, BYTE *data);
extern void		UD_free		(SPWAW_UD *ud);

#define	UD_ADD(ud_, strptr_, fld_)	\
	UD_add (ud_, (DWORD)&(strptr_##->##fld_) - (DWORD)strptr_, sizeof (strptr_##->##fld_), (BYTE *)strptr_##->##fld_)

#endif	/* UD_H */
