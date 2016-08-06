/** \file
 * Resource Archive - buildres tool - argument parsing.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	CONFIG_H
#define	CONFIG_H	1

typedef struct s_CONFIG {
	char	*cfg;
	char	*outlib;
	char	*outhdr;
	char	*outsrc;
} CONFIG;

extern CONFIG cfg;

extern void usage (char *, char *);
extern void parse_args (int, char**);

#endif	/* CONFIG_H */

