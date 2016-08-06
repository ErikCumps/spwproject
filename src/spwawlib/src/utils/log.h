/** \file
 * The SPWaW Library - utility code: logging.
 *
 * Copyright (C) 2007-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	LOG_H
#define	LOG_H	1

extern void	log_init	(char *log, bool append=false);
extern void	log		(char *fmt, ...);
extern void	log_nots	(char *fmt, ...);
extern void	log_shutdown	(void);

#endif	/* LOG_H */
