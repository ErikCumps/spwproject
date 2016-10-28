/** \file
 * The SPWaW war cabinet - resource handling - RGBA colors.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef RES_RGBA_H
#define RES_RGBA_H	1

#include "res/res_private.h"

extern SL_ERROR		RES_RGBA_init		(void);
extern void		RES_RGBA_shutdown	(void);

extern SL_ERROR		RES_RGBA_handle		(RESITEM *item);
extern QColor *		RES_RGBA_get		(RESITEM *item);
extern QString		RES_RGBA_gethtml	(RESITEM *item);
extern void		RES_RGBA_cleanup	(RESITEM *item);

extern void		RES_RGBA_statereport	(RESITEM *item);

#endif	/* RES_RGBA_H */
