/** \file
 * Slay's Library - internal basic output handling.
 *
 * Copyright (C) 2006-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef BASIC_OUTPUT_H
#define BASIC_OUTPUT_H	1

#include <SL/init_ids.h>

#ifdef	__cplusplus
extern "C" {
#endif	/* __cplusplus */


/* --- API --- */

extern SL_SINIT	SLBO_init	(void);
extern void	SLBO_shutdown	(void);


#ifdef	__cplusplus
}
#endif	/* __cplusplus */
#endif /* BASIC_OUTPUT_H */
