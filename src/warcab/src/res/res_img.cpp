/** \file
 * The SPWaW war cabinet - resource handling - Qt-supported images.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "res/res_img.h"

/*! Module name */
static const char	*MODULE = "RES_IMG";

SL_ERROR
RES_IMG_init (void)
{
	RETURN_OK;
}

void
RES_IMG_shutdown (void)
{
}

SL_ERROR
RES_IMG_handle (RESITEM *item)
{
	QPixmap	*pm = NULL;

	SL_CWENULLARG(item);
	SL_CWENULLARG(item->info);
	DEVASSERT (item->info->type == arc_IMG);

	pm = new QPixmap; DEVASSERT (pm != NULL);
	if (!pm->loadFromData ((const uchar *)item->item->data, item->item->size)) {
		delete pm; pm = NULL;
	}
	item->data = (void *)pm;

	RETURN_OK;
}

QPixmap *
RES_IMG_get (RESITEM *item)
{
	if (!item) return (NULL);

	return ((QPixmap *)item->data);
}

void
RES_IMG_cleanup (RESITEM *item)
{
	QPixmap	*pm;

	if (!item) return;

	pm = (QPixmap *)item->data; item->data = NULL;
	if (!pm) return;
	delete (pm);
}

void
RES_IMG_statereport (RESITEM *item)
{
	QPixmap	*pm = NULL;

	DEVASSERT (item);
	DEVASSERT (item->info);
	DEVASSERT (item->info->type == arc_IMG);

	pm = (QPixmap *)item->data;
	if (pm)
		SAYSTATE3 ("\t\t\t\t(%d x %d, %d bpp)\n", pm->width(), pm->height(), pm->depth());
	else
		SAYSTATE0 ("\t\t\t\t(NULL)\n");
}
