/** \file
 * The SPWaW war cabinet - resource handling - collections of Qt-supported images.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "res_imgcol.h"

typedef struct s_IMGCOL_DATA {
	unsigned long	iw;
	unsigned long	ih;
	unsigned long	rows;
	unsigned long	cols;
	QPixmap		**res;
} IMGCOL_DATA;

/*! Module name */
static const char	*MODULE = "RES_IMGCOL";

SL_ERROR
RES_IMGCOL_init (void)
{
	RETURN_OK;
}

void
RES_IMGCOL_shutdown (void)
{
}

SL_ERROR
RES_IMGCOL_handle (RESITEM *item)
{
	QPixmap		*resource = NULL;
	unsigned long	rw, rh, r, c;
	IMGCOL_DATA	*dp = NULL;

	SL_CWENULLARG(item);
	SL_CWENULLARG(item->info);
	DEVASSERT (item->info->type == arc_IMGCOL);

	resource = new QPixmap;
	if (resource && !resource->loadFromData ((const uchar *)item->item->data, item->item->size)) {
		delete resource; resource = NULL;
	}
	DEVASSERT (resource != NULL);

	rw = resource->width();
	rh = resource->height();

	SL_SAFE_CALLOC (dp, 1, sizeof (IMGCOL_DATA)); // FIXME: dp could be NULL in case of OOM!
	dp->iw = item->info->meta[0]; DEVASSERT (dp->iw != 0);
	dp->ih = item->info->meta[1]; DEVASSERT (dp->ih != 0);

	dp->cols = rw / dp->iw; if (rw % dp->iw) dp->cols++;
	dp->rows = rh / dp->ih; if (rh % dp->iw) dp->rows++;

	SL_SAFE_CALLOC (dp->res, dp->cols * dp->rows, sizeof (QPixmap *)); // FIXME: dp->res could be NULL in case of OOM!

	for (r=0; r<dp->rows; r++) {
		for (c=0; c<dp->cols; c++) {
			dp->res[r*dp->cols+c] = new QPixmap (resource->copy (c * dp->iw, r * dp->ih, dp->iw, dp->ih));
			DEVASSERT (dp->res[r*dp->cols+c] != NULL);
			dp->res[r*dp->cols+c]->setMask (dp->res[r*dp->cols+c]->createHeuristicMask());
		}
	}

	item->data = (void *)dp;
	delete (resource);

	RETURN_OK;
}

QPixmap *
RES_IMGCOL_get (RESITEM *item, unsigned long row, unsigned long col)
{
	IMGCOL_DATA	*dp;

	if (!item || !item->data) return (NULL);
	dp = (IMGCOL_DATA *)item->data;

	if ((row >= dp->rows) || (col >= dp->cols)) return (NULL);

	return (dp->res[row*dp->cols+col]);
}

void
RES_IMGCOL_cleanup (RESITEM *item)
{
	IMGCOL_DATA	*dp;
	unsigned long	r, c;

	if (!item || !item->data) return;
	dp = (IMGCOL_DATA *)item->data; item->data = NULL;

	for (r=0; r<dp->rows; r++) {
		for (c=0; c<dp->cols; c++) {
			delete (dp->res[r*dp->cols+c]);
			dp->res[r*dp->cols+c] = NULL;
		}
	}
	free (dp->res); dp->res = NULL;
	free (dp);
}

void
RES_IMGCOL_statereport (RESITEM *item)
{
	IMGCOL_DATA	*dp;

	DEVASSERT (item);
	DEVASSERT (item->info);
	DEVASSERT (item->info->type == arc_IMGCOL);

	dp = (IMGCOL_DATA *)item->data;
	if (dp) {
		SAYSTATE4 ("\t\t\t\t%lu rows x %lu cols (%lu x %lu)\n", dp->rows, dp->cols, dp->iw, dp->ih);
		for (unsigned long r=0; r<dp->rows; r++) {
			for (unsigned long c=0; c<dp->cols; c++) {
				QPixmap *pm = dp->res[r*dp->cols+c];
				SAYSTATE6 ("\t\t\t\t\t[%3lu,%3lu] 0x%8.8x %lu x %lu, %lu bpp\n",
					r, c, pm, pm->width(), pm->height(), pm->depth());
			}
		}
	} else {
		SAYSTATE0 ("\t\t\t\t(NULL)\n");
	}
}
