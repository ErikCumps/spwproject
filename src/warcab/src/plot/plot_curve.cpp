/** \file
 * The SPWaW war cabinet - plot handling - curve-type plot.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_curve.h"

PlotCurve::PlotCurve (char *name, void *parent, PLOT_TYPE type, QColor *color, int alpha, int width)
	: QwtPlotCurve (name)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	SL_SAFE_STRDUP (d.name, name);
	d.type = type;
	d.prnt = parent;

	set (color, alpha, width);
	setRenderHint (QwtPlotItem::RenderAntialiased);

	SET_GUICLS_NOERR;
}

PlotCurve::~PlotCurve (void)
{
	DBG_TRACE_DESTRUCT;

	SL_SAFE_FREE (d.name);
}

void
PlotCurve::set (QColor *color, int alpha, int width)
{
	QColor	c;
	QPen	p;
	QBrush	b;

	c = color ? *color : *RES_color (RID_PLOT_FG);
	c.setAlpha (alpha);

	p.setColor (c);
	if (width) p.setWidth (width);
	setPen (p);

	if (d.type == PLOT_AREA) {
		b.setStyle (Qt::SolidPattern);
		b.setColor (c);
		setBrush (b);
	}
};

char *
PlotCurve::name (void)
{
	return (d.name);
}

void *
PlotCurve::parent (void)
{
	return (d.prnt);
}