/** \file
 * The SPWaW war cabinet - plot handling - curve-type plot.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_CURVE_H
#define PLOT_CURVE_H	1

#include "plot.h"

class PlotCurve	: public QwtPlotCurve
{

public:
	PlotCurve	(char *name, void *parent, PLOT_TYPE type = PLOT_LINE, QColor *color = NULL, int alpha = 255, int width = 0);
	~PlotCurve	(void);

	SL_ERROR	error_code;

public:
	void	set	(QColor *color, int alpha, int width);
	char	*name	(void);
	void	*parent	(void);

private:
	struct s_data {
		char		*name;
		PLOT_TYPE	type;
		void		*prnt;
	} d;
};

#endif /* PLOT_CURVE_H */
