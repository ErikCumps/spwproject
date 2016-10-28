/** \file
 * The SPWaW war cabinet - plot handling - symbols.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_SYMBOL_H
#define PLOT_SYMBOL_H	1

#include "plot.h"

class PlotSymbol	: public QwtSymbol
{

public:
	PlotSymbol	(QwtSymbol::Style style, QColor *pencolor = NULL, QColor *brushcolor = NULL, int xsize = 1, int ysize = 1, int width = 0);
	~PlotSymbol	(void);

	SL_ERROR	error_code;

};
#endif /* PLOT_SYMBOL_H */
