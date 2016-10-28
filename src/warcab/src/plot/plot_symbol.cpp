/** \file
 * The SPWaW war cabinet - plot handling - symbols.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_symbol.h"

PlotSymbol::PlotSymbol (QwtSymbol::Style style, QColor *pencolor, QColor *brushcolor, int xsize, int ysize, int width)
	: QwtSymbol ()
{
	QColor	pc, bc;
	QPen	p;
	QBrush	b;

	DBG_TRACE_CONSTRUCT;

	pc = pencolor ? *pencolor : *RES_color (RID_PLOT_FG);
	bc = brushcolor ? *brushcolor : *RES_color (RID_PLOT_BG);

	setStyle (style);
	setSize (xsize, ysize);

	p.setColor (pc);
	if (width) p.setWidth (width);
	setPen (p);

	b.setStyle (Qt::SolidPattern);
	b.setColor (bc);
	setBrush (b);

	SET_GUICLS_NOERR;
}

PlotSymbol::~PlotSymbol (void)
{
	DBG_TRACE_DESTRUCT;
}
