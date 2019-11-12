/** \file
 * The SPWaW war cabinet - resource handling - XPM images.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "res/res_xpm.h"

/*! Module name */
static const char	*MODULE = "RES_XPM";

SL_ERROR
RES_XPM_init (void)
{
	RETURN_OK;
}

void
RES_XPM_shutdown (void)
{
}

SL_ERROR
RES_XPM_handle (RESITEM *item)
{
	unsigned long	*xpm_cnt;
	unsigned long	*xpm_lst;
	char		*xpm_str;
	char		**xpm = NULL;
	unsigned long	i;
	QPixmap		*pm = NULL;

	SL_CWENULLARG(item);
	SL_CWENULLARG(item->info);
	DEVASSERT (item->info->type == arc_XPM);

	xpm_cnt = (unsigned long *)((char *)item->item->data + 0);
	xpm_lst = (unsigned long *)((char *)item->item->data + sizeof (unsigned long));
	xpm_str = (char *)item->item->data + sizeof (unsigned long)*(1+*xpm_cnt);

	SL_SAFE_CALLOC (xpm, *xpm_cnt, sizeof (char *)); // FIXME: xpm could be NULL in case of OOM!
	for (i=0; i<*xpm_cnt; i++) {
		xpm[i] = xpm_lst[i] + xpm_str;
	}

	pm = new QPixmap (xpm); DEVASSERT (pm != NULL);
	item->data = (void *)pm;

	SL_SAFE_FREE (xpm);

	RETURN_OK;
}

QPixmap *
RES_XPM_get (RESITEM *item)
{
	if (!item) return (NULL);

	return ((QPixmap *)item->data);
}

void
RES_XPM_cleanup (RESITEM *item)
{
	QPixmap	*pm;

	if (!item) return;

	pm = (QPixmap *)item->data; item->data = NULL;
	if (!pm) return;
	delete (pm);
}

void
RES_XPM_statereport (RESITEM *item)
{
	QPixmap	*pm = NULL;

	DEVASSERT (item);
	DEVASSERT (item->info);
	DEVASSERT (item->info->type == arc_XPM);

	pm = (QPixmap *)item->data;
	if (pm)
		SAYSTATE3 ("\t\t\t\t(%d x %d, %d bpp)\n", pm->width(), pm->height(), pm->depth());
	else
		SAYSTATE0 ("\t\t\t\t(NULL)\n");
}
