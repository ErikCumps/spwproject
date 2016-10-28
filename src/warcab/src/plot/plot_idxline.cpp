/** \file
 * The SPWaW war cabinet - plot handling - axis index labels.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "plot_idxline.h"
#include "plot_timeline.h"

PlotIdxline::PlotIdxline (int *cnt_ptr, double **lst_ptr)
	: QwtScaleDraw ()
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (cnt_ptr && lst_ptr) {
		d.cnt = cnt_ptr;
		d.lst = lst_ptr;
	}

	SET_GUICLS_NOERR;
}

PlotIdxline::~PlotIdxline (void)
{
	DBG_TRACE_DESTRUCT;
}

QwtText
PlotIdxline::label (double v) const
{
	int		idx;
	SPWAW_DATE	date;
	QString		s;

	idx = (int)floor(v);
	if ((idx >= 0) && d.cnt && (idx < *(d.cnt)) && d.lst && (*(d.lst))) {
		PLOT_timeline2date ((*(d.lst))[idx], date);
		s.sprintf ("%d/%d", date.month, date.year - 1900);
	}
	return (s);
}
