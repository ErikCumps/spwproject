/** \file
 * The SPWaW war cabinet - resource handling - Qt-supported images.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef RES_IMG_H
#define RES_IMG_H	1

#include "res/res_private.h"

extern SL_ERROR		RES_IMG_init		(void);
extern void		RES_IMG_shutdown	(void);

extern SL_ERROR		RES_IMG_handle		(RESITEM *item);
extern QPixmap *	RES_IMG_get		(RESITEM *item);
extern void		RES_IMG_cleanup		(RESITEM *item);

extern void		RES_IMG_statereport	(RESITEM *item);

#endif	/* RES_IMG_H */
