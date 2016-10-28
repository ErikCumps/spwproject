/** \file
 * The SPWaW war cabinet - plot handling - legend.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_LEGEND_H
#define PLOT_LEGEND_H	1

#include "plot.h"

class PlotLegend	: public QwtLegend
{
public:
	PlotLegend	(bool active = false);
	~PlotLegend	(void);

	SL_ERROR	error_code;
};


#endif /* PLOT_LEGEND_H */
