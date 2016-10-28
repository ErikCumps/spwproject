/** \file
 * The SPWaW war cabinet - plot handling - grid.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_grid.h"
#include "plot_private.h"

PlotGrid::PlotGrid (QColor *color)
	: QwtPlotGrid()
{
	QColor	c;
	QPen	p;

	DBG_TRACE_CONSTRUCT;

	c = color ? *color : *RES_color (RID_PLOT_GRID);

	p.setColor (c);
	p.setStyle (Qt::DashLine);
	p.setCosmetic (true);
	setPen (p);

	setZ (PLOT_Z_GRID);

	SET_GUICLS_NOERR;
}

PlotGrid::~PlotGrid (void)
{
	DBG_TRACE_DESTRUCT;
}
