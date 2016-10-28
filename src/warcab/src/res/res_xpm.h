/** \file
 * The SPWaW war cabinet - resource handling - XPM images.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef RES_XPM_H
#define RES_XPM_H	1

#include "res/res_private.h"

extern SL_ERROR		RES_XPM_init		(void);
extern void		RES_XPM_shutdown	(void);

extern SL_ERROR		RES_XPM_handle		(RESITEM *item);
extern QPixmap *	RES_XPM_get		(RESITEM *item);
extern void		RES_XPM_cleanup		(RESITEM *item);

extern void		RES_XPM_statereport	(RESITEM *item);

#endif	/* RES_XPM_H */
