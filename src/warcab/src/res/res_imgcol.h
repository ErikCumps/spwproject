/** \file
 * The SPWaW war cabinet - resource handling - collections of Qt-supported images.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef RES_IMGCOL_H
#define RES_IMGCOL_H	1

#include "res/res_private.h"

extern SL_ERROR		RES_IMGCOL_init		(void);
extern void		RES_IMGCOL_shutdown	(void);

extern SL_ERROR		RES_IMGCOL_handle	(RESITEM *item);
extern QPixmap *	RES_IMGCOL_get		(RESITEM *item, unsigned long row, unsigned long col);
extern void		RES_IMGCOL_cleanup	(RESITEM *item);

extern void		RES_IMGCOL_statereport	(RESITEM *item);

#endif	/* RES_IMGCOL_H */
