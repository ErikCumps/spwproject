/** \file
 * Resource Archive - buildres tool - ID (value) hash handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	IDHASH_H
#define	IDHASH_H	1

extern void ih_init (void);
extern void ih_shutdown (void);
extern void *ih_find (unsigned long);
extern void ih_register (unsigned long);

#endif	/* IDHASH_H */

