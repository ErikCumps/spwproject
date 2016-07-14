/** \file
 * Slay's Library - main header file.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_H
#define	SL_H	1

#include <SL/init_ids.h>
#include <SL/callbacks.h>
#include <SL/system.h>
#include <SL/safe_mem.h>
#include <SL/bool.h>
#include <SL/lock.h>
#include <SL/util.h>
#include <SL/message.h>
#include <SL/stdbg.h>
#include <SL/thread.h>
#include <SL/app.h>
#include <SL/app_exit.h>
#include <SL/error.h>

#include <SL/opt.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */

extern SL_INIT	SL_init		(SL_APP_INFO *info, SLOPT *opt);
extern SL_INIT	SL_lib_init	(void);
extern void	SL_shutdown	(void);
extern void	SL_lib_shutdown	(void);

#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_H */