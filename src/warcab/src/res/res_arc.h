/** \file
 * The SPWaW war cabinet - resource archive handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef RES_ARCHIVE_H
#define RES_ARCHIVE_H	1

extern SL_ERROR	RES_ARC_init		(char *filename);
extern void	RES_ARC_shutdown	(void);

extern SL_ERROR	RES_ARC_item		(int id, arc_item_t **item);

#endif	/* RES_ARCHIVE_H */
