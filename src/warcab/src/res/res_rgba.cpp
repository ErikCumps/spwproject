/** \file
 * The SPWaW war cabinet - resource handling - RGBA colors.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "res/res_rgba.h"

/*! Module name */
static const char	*MODULE = "RES_RGBA";

SL_ERROR
RES_RGBA_init (void)
{
	RETURN_OK;
}

void
RES_RGBA_shutdown (void)
{
}

SL_ERROR
RES_RGBA_handle (RESITEM *item)
{
	QColor		*c = NULL;
	unsigned long	*d;

	SL_CWENULLARG(item);
	SL_CWENULLARG(item->info);
	DEVASSERT (item->info->type == arc_RGBA);

	c = new QColor; DEVASSERT (c != NULL);
	d = (unsigned long *)item->item->data;
	c->setRed (d[0]); c->setGreen (d[1]); c->setBlue (d[2]); c->setAlpha (d[3]);
	item->data = (void *)c;

	RETURN_OK;
}

QColor *
RES_RGBA_get (RESITEM *item)
{
	if (!item) return (NULL);

	return ((QColor *)item->data);
}

QString
RES_RGBA_gethtml (RESITEM *item)
{
	QString	c = QString();

	if (item) {
		unsigned long *d = (unsigned long *)item->item->data;
		c.sprintf ("#%02.2X%02.2X%02.2X", d[0] & 0xFF, d[1] & 0xFF, d[2] & 0xFF);
	}
	return (c);
}

void
RES_RGBA_cleanup (RESITEM *item)
{
	QColor	*c;

	if (!item) return;

	c = (QColor *)item->data; item->data = NULL;
	if (!c) return;
	delete c;
}

void
RES_RGBA_statereport (RESITEM *item)
{
	QColor	*c = NULL;

	DEVASSERT (item);
	DEVASSERT (item->info);
	DEVASSERT (item->info->type == arc_RGBA);

	c = (QColor *)item->data;
	if (c)
		SAYSTATE4 ("\t\t\t\t(R=%d, G=%d, B=%d, A=%d)\n", c->red(), c->green(), c->blue(), c->alpha());
	else
		SAYSTATE0 ("\t\t\t\t(NULL)\n");
}
