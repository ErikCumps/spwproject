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

#define sazslen(azs_)		azslen (azs_, sizeof(azs_))
#define	sazsclr(azs_)		azsclr (azs_, sizeof(azs_))
#define	sazsset(azs_, string_)	azsset (azs_, sizeof(azs_), string_)
#define	sazsdup(azs_)		azsdup (azs_, sizeof(azs_))
#define	sazscpy(src_, dst_)	azscpy (src_, dst_, sizeof(dst_))

extern DWORD	azslen	(char *azs, DWORD size);
extern void	azsclr	(char *azs, DWORD size);
extern void	azsset	(char *azs, DWORD size, char *string);
extern char *	azsdup	(char *azs, DWORD size);
extern void	azscpy	(char *src, char *dst, DWORD size);

#endif	/* AZS_H */
