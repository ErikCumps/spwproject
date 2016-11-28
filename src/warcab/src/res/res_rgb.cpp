/** \file
 * The SPWaW war cabinet - resource handling - RGB colors.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "res/res_rgb.h"

/*! Module name */
static const char	*MODULE = "RES_RGB";

SL_ERROR
RES_RGB_init (void)
{
	RETURN_OK;
}

void
RES_RGB_shutdown (void)
{
}

SL_ERROR
RES_RGB_handle (RESITEM *item)
{
	QColor		*c = NULL;
	unsigned long	*d;

	SL_CWENULLARG(item);
	SL_CWENULLARG(item->info);
	DEVASSERT (item->info->type == arc_RGB);

	c = new QColor; DEVASSERT (c != NULL);
	d = (unsigned long *)item->item->data;
	c->setRed (d[0]); c->setGreen (d[1]); c->setBlue (d[2]);
	item->data = (void *)c;

	RETURN_OK;
}

QColor *
RES_RGB_get (RESITEM *item)
{
	if (!item) return (NULL);

	return ((QColor *)item->data);
}

QString
RES_RGB_gethtml (RESITEM *item)
{
	QString	c = QString();

	if (item) {
		unsigned long *d = (unsigned long *)item->item->data;
		c.sprintf ("#%02.2X%02.2X%02.2X", d[0] & 0xFF, d[1] & 0xFF, d[2] & 0xFF);
	}
	return (c);
}

void
RES_RGB_cleanup (RESITEM *item)
{
	QColor	*c;

	if (!item) return;

	c = (QColor *)item->data; item->data = NULL;
	if (!c) return;
	delete (c);
}

void
RES_RGB_statereport (RESITEM *item)
{
	QColor	*c = NULL;

	DEVASSERT (item);
	DEVASSERT (item->info);
	DEVASSERT (item->info->type == arc_RGB);

	c = (QColor *)item->data;
	if (c)
		SAYSTATE3 ("\t\t\t\t(R=%d, G=%d, B=%d)\n", c->red(), c->green(), c->blue());
	else
		SAYSTATE0 ("\t\t\t\t(NULL)\n");
}
