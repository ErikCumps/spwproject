/** \file
 * The SPWaW war cabinet - plot handling - bar-type plot.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

/* A bar plot item, based on the QwtPlotCurve code */

#ifndef PLOT_BAR_ITEM_H
#define PLOT_BAR_ITEM_H	1

#include "plot.h"

class PlotBarItem	: public QwtPlotItem
{

public:
	enum PlotType {
		Yfx,		/* Draw bars along y axis. Baseline is parallel with x axis. */
		Xfy		/* Draw bars along X axis. Baseline is parallel with y axis. */
	};

public:
	explicit	PlotBarItem	(void);
	explicit	PlotBarItem	(const QwtText &title);
	explicit	PlotBarItem	(const QString &title);
	virtual		~PlotBarItem	(void);

public:
	int		rtti		(void)	const;

public:
	void		setPlotType	(PlotType type);
	PlotType	plotType	(void)	const;
	void		setBaseline	(double base);
	double		baseline	(void)	const;
	void		setPen		(const QPen &pen);
	const QPen &	pen		(void)	const;
	void		setBrush	(const QBrush &brush);
	const QBrush &	brush		(void)	const;
	void		setSymbol	(const QwtSymbol &symbol);
	const QwtSymbol&symbol		(void) const;
	void		setDodge	(double dodge);
	double		dodge		(void) const;
	void		setWidth	(double width);
	double		width		(void) const;

public:
	void		setRawData	(const double *x, const double *y, int size);
	void		setData		(const double *x, const double *y, int size);
	void		setData		(const QwtArray<double>&x, const QwtArray<double>&y);
	void		setData		(const QPolygonF &data);
	void		setData		(const QwtData &data);

	QwtData &	data		(void);
	const QwtData &	data		(void)	const;

	int		dataSize	(void)	const;
	double		x		(int i)	const;
	double		y		(int i)	const;

public:
	QwtDoubleRect	boundingRect	(void)	const;
	double		minXValue	(void)	const;
	double		maxXValue	(void)	const;
	double		minYValue	(void)	const;
	double		maxYValue	(void)	const;

public:
	void		draw		(QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, const QRect &r)	const;
	void		draw		(QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, int r0, int r1)	const;
	void		draw		(int r0, int r1)								const;
	void		updateLegend	(QwtLegend *l)									const;

public:
	int		closestPoint	(const QPoint &pos, double *dist = NULL)	const;

protected:
	void		init		(void);
	void		drawBars	(QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, int r0, int r1)	const;
	//void		fillCurve	(QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, QwtPolygon &pol)	const;
	//void		closePolyline	(const QwtScaleMap &xm, const QwtScaleMap &ym, QwtPolygon &pol)			const;

private:
	class PrivateData;

	QwtData		*d_xy;
	PrivateData	*d_data;
};






#endif /* PLOT_BAR_ITEM_H */
