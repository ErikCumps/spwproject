/** \file
 * The SPWaW war cabinet - plot handling - background.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_BACKGROUND_H
#define PLOT_BACKGROUND_H	1

#include "plot.h"

class PlotBG	: public QwtPlotItem
{

public:
	PlotBG	(QColor *color = NULL);
	~PlotBG	(void);

	SL_ERROR	error_code;

public:
	virtual int	rtti	(void);
	virtual void	draw	(QPainter *p, const QwtScaleMap &x, const QwtScaleMap &y, const QRect &r) const;

private:
	struct s_data {
		QColor	c;
	} d;
};

#endif /* PLOT_BACKGROUND_H */
