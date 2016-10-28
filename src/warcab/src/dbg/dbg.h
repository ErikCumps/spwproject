/** \file
 * The SPWaW war cabinet - debugging support.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	DBG_COMMON_H
#define	DBG_COMMON_H	1

#include <build_options.h>

extern void	DBG_init	(void);
extern void	DBG_shutdown	(void);
extern void	DBG_log		(char *fmt, ...);
extern void	DBG_logbuf	(char *buf, int len);
extern void	DBG_oom_hook	(const char *file, unsigned long line, const char *func, unsigned long size);
extern void	DBG_panic_hook	(void);

#if	DEBUG_TRACE

#define	DBG_TRACE_FENTER	do { DBG_log ("<%s> ENTER\n", __FUNCTION__); } while (0)
#define	DBG_TRACE_FLEAVE	do { DBG_log ("<%s> LEAVE\n", __FUNCTION__); } while (0)
#define	DBG_TRACE_CONSTRUCT	do { DBG_log ("<%s> CONSTRUCT [0x%8.8x]\n", __FUNCTION__, this); } while (0)
#define	DBG_TRACE_DESTRUCT	do { DBG_log ("<%s> DESTRUCT [0x%8.8x]\n", __FUNCTION__, this); } while (0)
#define	DBG_TRACE_UPDATE	do { DBG_log ("<%s> UPDATING GUI DATA\n", __FUNCTION__); } while (0)

#else	/* !DEBUG_TRACE */

#define	DBG_TRACE_FENTER
#define	DBG_TRACE_FLEAVE
#define	DBG_TRACE_CONSTRUCT
#define	DBG_TRACE_DESTRUCT
#define	DBG_TRACE_UPDATE

#endif	/* !DEBUG_TRACE */

#endif	/* DBG_COMMON_H */
