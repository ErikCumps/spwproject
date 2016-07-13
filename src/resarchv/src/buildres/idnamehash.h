/** \file
 * Resource Archive - buildres tool - ID (name) hash handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	IDNAMEHASH_H
#define	IDNAMEHASH_H	1

extern void nh_init (void);
extern void nh_shutdown (void);
extern void *nh_find (char *);
extern void nh_register (char *, unsigned long);

#endif	/* IDNAMEHASH_H */

