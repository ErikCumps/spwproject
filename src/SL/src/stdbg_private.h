/** \file
 * Slay's Library - state debug reporting.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	SL_STDBG_PRIVATE_H
#define	SL_STDBG_PRIVATE_H	1

#include <SL/stdbg.h>
#include <SL/init_ids.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */



/* --- API --- */

extern SL_SINIT	SL_STDBG_init		(void);
extern void	SL_STDBG_shutdown	(void);



#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif	/* SL_STDBG_PRIVATE_H */

