/** \file
 * The SPWaW war cabinet - plot handling - axis timeline labels.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "plot_timeline.h"

PlotTimeline::PlotTimeline (void)
	: QwtScaleDraw ()
{
	DBG_TRACE_CONSTRUCT;

	SET_GUICLS_NOERR;
}

PlotTimeline::~PlotTimeline (void)
{
	DBG_TRACE_DESTRUCT;
}

QwtText
PlotTimeline::label (double v) const
{
	SPWAW_DATE	date;
	QString		s;

	PLOT_timeline2date (v, date);
	if (scaleDiv().range() > 31.0) {
		s.sprintf ("%d/%d", date.month, date.year - 1900);
	} else if (scaleDiv().range() > 1.0) {
		s.sprintf ("%d/%d/%d", date.day, date.month, date.year - 1900);
	} else {
		s.sprintf ("%2d:%02d", date.hour, date.minute);
	}
	return (s);
}

void
PLOT_date2timeline (SPWAW_DATE &date, double &timeline)
{
	timeline = 0.0;

	if (date.year < 1900) return;
	timeline += (double)(date.year - 1900) * (12 * 31);

	if (date.month < 1) return;
	timeline += (date.month - 1) * 31;

	if (date.day < 1) return;
	timeline += (double)(date.day - 1);

	if ((date.hour < 0) || (date.minute < 0)) return;
	timeline += (double)(date.hour * 60 + date.minute) / (24*60);
}

void
PLOT_timeline2date (double &timeline, SPWAW_DATE &date)
{
	int	ym, hm;
	double	d, df, dc;

	memset (&date, 0, sizeof (date));

	if (timeline <= 0.0) return;

	ym = (int)floor(timeline) / 31;

	date.year  = (ym / 12) + 1900;
	date.month = (ym % 12) + 1;
	date.day   = ((int)floor(timeline) % 31) + 1;

	d = fmod (timeline, 1.0) * (24*60);
	df = floor (d); dc = ceil (d);
	hm = (int)((d - df) < (dc - d) ? df : dc);
	date.hour   = hm / 60;
	date.minute = hm % 60;
}
