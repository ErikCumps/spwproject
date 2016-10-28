/** \file
 * The SPWaW war cabinet - resource handling - private header file.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	RES_PRIVATE_H
#define	RES_PRIVATE_H	1

#include <resource.h>
#include "res/res.h"

typedef struct s_RESITEM {
	const RESOURCE_LIST_ELEMENT	*info;
	arc_item_t			*item;
	void				*data;
} RESITEM;

#endif	/* RES_PRIVATE_H */
