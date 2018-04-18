/** \file
 * The SPWaW Library - utility code: handling AZS strings.
 * (these are max length, possibly zero-terminated, ascii strings)
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	AZS_H
#define	AZS_H	1

extern char *	azsdup	(char *string, DWORD len);
extern void	azscpy	(char *src, char *dst, unsigned int size);

#endif	/* AZS_H */
