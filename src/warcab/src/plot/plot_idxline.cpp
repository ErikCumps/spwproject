/** \file
 * The SPWaW war cabinet - plot handling - axis index labels.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
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
	double		range;
	QString		s;

	idx = (int)floor(v);
	if ((idx >= 0) && d.cnt && (idx < *(d.cnt)) && d.lst && (*(d.lst))) {
		PLOT_timeline2date ((*(d.lst))[idx], date);

		if (!PLOT_hasBtlidx (date)) {
			range = (*(d.lst))[*(d.cnt)-1] - (*(d.lst))[0];

			if (range > 31.0) {
				s.sprintf ("%d/%d", date.month, date.year - 1900);
			} else if (range > 1.0) {
				s.sprintf ("%d/%d/%d", date.day, date.month, date.year - 1900);
			} else {
				s.sprintf ("%2d:%02d", date.hour, date.minute);
			}

			//DBG_log ("%s - 0x%8.8x - range = %f - label=\"%s\"", __FUNCTION__, this, range, qPrintable (s));
		} else {
			s.sprintf (" #%d %d/%d", PLOT_decodeBtlidx(date)+1, date.month, date.year - 1900);

			//DBG_log ("%s - 0x%8.8x - label=\"%s\"", __FUNCTION__, this, qPrintable (s));
		}
	}
	return (s);
}
