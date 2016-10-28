/** \file
 * The SPWaW war cabinet - plot handling - markers.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_MARK_H
#define PLOT_MARK_H	1

#include "plot.h"

class PlotMark	: public QwtPlotMarker
{

public:
	PlotMark	(int spacing = 0);
	~PlotMark	(void);

	SL_ERROR	error_code;

public:
	void	setText	(QString &s);
	void	setAlign(Qt::Alignment align);
	void	refresh	(void);

	void	draw	(QPainter *p, const QwtScaleMap &x, const QwtScaleMap &y, const QRect &c) const;

private:
	struct s_data {
		QwtText	*text;
	} d;

private:
	void	drawme	(QPainter *p, const QRect &c, const QPoint &xy) const;
};

#endif /* PLOT_MARK_H */
