/** \file
 * The SPWaW war cabinet - plot handling - min-max-average-spread plot handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_MMAS_H
#define PLOT_MMAS_H	1

#include "plot.h"
#include "model/model_mmas.h"

typedef struct s_PlotMMASCurve {
	PlotCurve		*curve;
	PlotSymbol		*symbol;
	double			*data;
	MDLMMAS_COLUMN		col;
} PlotMMASCurve;

typedef struct s_PlotMMASSet {
	int			count;
	double			*time;
	double			*axis;
	PlotMMASCurve		max;
	PlotMMASCurve		min;
	PlotMMASCurve		avg;
	PlotMMASCurve		spr;
	PlotMMASCurve		cnt;
} PlotMMASSet;

class PlotMMAS	: public QwtPlot
{
	Q_OBJECT

public:
	PlotMMAS	(ModelMMAS *model, QWidget *P = 0);
	~PlotMMAS	(void);

	SL_ERROR	error_code;

public:
	void		refresh		(void);
	virtual bool	eventFilter	(QObject *obj, QEvent *e);
	void		set_axistype	(AXIS_TYPE type);

private:
	struct s_data {
		PlotBG		*p_bg;
		PlotTimeline	*p_timeline;
		PlotIdxline	*p_idxline;
		PlotGrid	*p_grid;
		PlotLegend	*p_legend;
		PlotMMASSet	set;
		PlotMark	*mark;
		ModelMMAS	*model;
		AXIS_TYPE	axis_type;
	} d;

private:
	void	clear		(void);
	void	update		(void);
	void	track		(const QPoint &);
	void	set_scaledraw	(AXIS_TYPE type);
	void	enable_curve	(QwtPlotItem *, bool on);

private slots:
	void	show		(QwtPlotItem *, bool on);
};

#endif /* PLOT_MMAS_H */
