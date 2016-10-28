/** \file
 * The SPWaW war cabinet - plot handling - table plot handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#ifndef PLOT_TABLE_H
#define PLOT_TABLE_H	1

#include "plot.h"
#include "model/model_table.h"

typedef enum e_PLOTTABLE_TYPE {
	PLOTTABLE_CURVE = 0,
	PLOTTABLE_BAR,
} PLOTTABLE_TYPE;

typedef struct s_PlotTableCurve	PlotTableCurve;
typedef struct s_PlotTableSet	PlotTableSet;

struct s_PlotTableCurve {
	PlotTableSet		*set;
	int			idx;
	PLOTTABLE_TYPE		type;
	union u_u {
		PlotCurve	*curve;
		PlotBar		*bar;
	} u;
	PlotSymbol		*symbol;
	double			*data;
	double			*reld;
	bool			empty;
};

struct s_PlotTableSet {
	bool			stacked;
	double			*time;
	double			*axis;
	int			curve_cnt;
	PlotTableCurve		*curve;
	int			row_cnt;
};

class PlotTable	: public QwtPlot
{
	Q_OBJECT

public:
	PlotTable	(ModelTable *model, QWidget *P);
	~PlotTable	(void);

	SL_ERROR	error_code;

public:
	virtual bool	eventFilter	(QObject *obj, QEvent *e);

public slots:
	void		commit		(void);
	void		commit_def	(void);
	void		commit_dat	(void);

private:
	struct s_data {
		ModelTable	*model;
		PlotBG		*p_bg;
		PlotTimeline	*p_timeline;
		PlotIdxline	*p_idxline;
		PlotGrid	*p_grid;
		PlotLegend	*p_legend;
		PlotMark	*mark;
		PlotTableSet	set;
		MDLT_DEF	*cfg;
		AXIS_TYPE	axis_type;
	} d;

private:
	void	setup		(void);
	void	setup_def	(void);
	void	setup_dat	(void);
	void	clear		(void);
	void	clear_def	(void);
	void	clear_dat	(void);
	void	plot		(void);
	void	track		(const QPoint &);
	void	set_axistype	(AXIS_TYPE type);
	void	set_scaledraw	(AXIS_TYPE type);
	void	enable_curve	(QwtPlotItem *, bool on);

private:
	void	curve_setdata	(int idx, const double *x, const double *y, int size);
	void	curve_setdodge	(int idx, double dodge);
	void	curve_setwidth	(int idx, double width);
	void	curve_show	(int idx);
	void	curve_delete	(int idx);
	double	curve_vx	(PlotTableCurve *curve, int idx);
	double	curve_vy	(PlotTableCurve *curve, int idx);
	double	curve_tol	(PlotTableCurve *curve);
	char	*curve_name	(PlotTableCurve *curve);

private slots:
	void	show		(QwtPlotItem *, bool on);
};

#endif /* PLOT_TABLE_H */
