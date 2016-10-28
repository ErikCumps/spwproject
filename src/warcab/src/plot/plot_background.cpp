/** \file
 * The SPWaW war cabinet - plot handling - background.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_background.h"
#include "plot_private.h"

PlotBG::PlotBG (QColor *color)
{
	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	d.c = color ? *color : *RES_color (RID_PLOT_BG);
	setZ (PLOT_Z_BACKGROUND);

	SET_GUICLS_NOERR;
}

PlotBG::~PlotBG (void)
{
	DBG_TRACE_DESTRUCT;
}

int
PlotBG::rtti (void)
{
	return (QwtPlotItem::Rtti_PlotUserItem);
}

void
PlotBG::draw (QPainter *p, const QwtScaleMap &/*x*/, const QwtScaleMap &/*y*/, const QRect &r) const
{
	p->fillRect (r, d.c);
}

