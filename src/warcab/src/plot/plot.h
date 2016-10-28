/** \file
 * The SPWaW war cabinet - plot handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef	PLOT_H
#define	PLOT_H	1

class	PlotBG;
class	PlotTimeline;
class	PlotIdxline;
class	PlotGrid;
class	PlotLegend;
class	PlotMark;
class	PlotSymbol;
class	PlotCurve;
class	PlotBar;
class	PlotMMAS;
class	PlotTable;

#include "../gui/gui_private.h"

typedef enum e_PLOT_TYPE {
	PLOT_LINE = 0,
	PLOT_AREA,
	PLOT_BAR,
	PLOT_TYPE_LIMIT = PLOT_BAR
} PLOT_TYPE;

typedef enum e_AXIS_TYPE {
	AXIS_TIME = 0,
	AXIS_INDEX,
	AXIS_TYPE_LIMIT = AXIS_INDEX
} AXIS_TYPE;

#endif	/* PLOT_H */
