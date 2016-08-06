/** \file
 * The SPWaW Library - utility code: file content verification.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	FILECHECK_H
#define	FILECHECK_H	1

/* Simple file verification */

extern SPWAW_ERROR	fcheck_make	(int fd);
extern SPWAW_ERROR	fcheck_verify	(int fd);

#endif	/* FILECHECK_H */
