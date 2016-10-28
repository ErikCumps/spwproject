/** \file
 * The SPWaW war cabinet - resource handling - RGB colors.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef RES_RGB_H
#define RES_RGB_H	1

#include "res/res_private.h"

extern SL_ERROR		RES_RGB_init		(void);
extern void		RES_RGB_shutdown	(void);

extern SL_ERROR		RES_RGB_handle		(RESITEM *item);
extern QColor *		RES_RGB_get		(RESITEM *item);
extern QString		RES_RGB_gethtml		(RESITEM *item);
extern void		RES_RGB_cleanup		(RESITEM *item);

extern void		RES_RGB_statereport	(RESITEM *item);

#endif	/* RES_RGB_H */
