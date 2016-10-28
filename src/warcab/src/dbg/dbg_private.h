/** \file
 * The SPWaW war cabinet - debugging support - private header file.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	DBG_PRIVATE_H
#define	DBG_PRIVATE_H	1

#include <SL/bool.h>

extern void	DBG_CONS_init		(void);
extern void	DBG_CONS_shutdown	(void);
extern void	DBG_CONS_log		(SL_BOOL cons, SL_BOOL file, char *fmt, ...);
extern void	DBG_CONS_vlog		(SL_BOOL cons, SL_BOOL file, char *fmt, va_list AP);
extern void	DBG_CONS_logbuf		(SL_BOOL cons, SL_BOOL file, char *buf, int len);
extern void	DBG_CONS_flush		(void);



#endif	/* DBG_PRIVATE_H */
