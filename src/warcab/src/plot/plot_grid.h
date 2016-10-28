/** \file
 * The SPWaW war cabinet - plot handling - grid.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_GRID_H
#define PLOT_GRID_H	1

#include "plot.h"

class PlotGrid	: public QwtPlotGrid
{
public:
	PlotGrid	(QColor *color = NULL);
	~PlotGrid	(void);

	SL_ERROR	error_code;
};


#endif /* PLOT_GRID_H */
