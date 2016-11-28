/** \file
 * The SPWaW war cabinet - plot handling - min-max-average-spread plot handling.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_mmas.h"
#include "plot_background.h"
#include "plot_grid.h"
#include "plot_timeline.h"
#include "plot_idxline.h"
#include "plot_legend.h"
#include "plot_symbol.h"
#include "plot_curve.h"
#include "plot_mark.h"
#include "plot_private.h"

#define	Z_MAX		(PLOT_Z_USER + 4)
#define	Z_MIN		(PLOT_Z_USER + 3)
#define	Z_AVG		(PLOT_Z_USER + 5)
#define	Z_SPR		(PLOT_Z_USER + 2)
#define	Z_CNT		(PLOT_Z_USER + 1)

#define	T_DAY		(1.0)
#define	T_QUARTER	(T_DAY * 31 * 3)
#define	S_SMALL		(1.0/24.0/12.0)

#define	Y_ALIGN		20

#define	M_SPACE		10
#define	M_TOL		25

/* Convenience macro */
#define	DOUBLEPREC	1.0e-6

PlotMMAS::PlotMMAS (ModelMMAS *model, QWidget *P)
	: QwtPlot (P)
{
	PlotCurve	*C = NULL;
	PlotSymbol	*S = NULL;
	QwtScaleWidget	*sw = NULL;
	QwtScaleDraw	*sd = NULL;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	/* background */
	GUINEW (d.p_bg, PlotBG(), ERR_GUI_PLOT_INIT_FAILED, "p_bg");
	GUIERR (d.p_bg, ERR_GUI_PLOT_INIT_FAILED);
	d.p_bg->attach (this);

	/* scale drawer for time axis */
	GUINEW (d.p_timeline, PlotTimeline(), ERR_GUI_PLOT_INIT_FAILED, "p_timeline");
	GUIERR (d.p_timeline, ERR_GUI_PLOT_INIT_FAILED);

	/* scale drawer for index axis */
	GUINEW (d.p_idxline, PlotIdxline(&(d.set.count), &(d.set.time)), ERR_GUI_PLOT_INIT_FAILED, "p_idxline");
	GUIERR (d.p_idxline, ERR_GUI_PLOT_INIT_FAILED);

	/* grid */
	GUINEW (d.p_grid, PlotGrid(), ERR_GUI_PLOT_INIT_FAILED, "p_grid");
	GUIERR (d.p_grid, ERR_GUI_PLOT_INIT_FAILED);
	d.p_grid->attach (this);

	/* legend */
	GUINEW (d.p_legend, PlotLegend (true), ERR_GUI_PLOT_INIT_FAILED, "p_legend");
	GUIERR (d.p_legend, ERR_GUI_PLOT_INIT_FAILED);
	//p_legend->setItemMode (QwtLegend::ReadOnlyItem);
	insertLegend (d.p_legend, QwtPlot::RightLegend);

	/* max symbol and curve */
	GUINEW (S, PlotSymbol (QwtSymbol::Ellipse, RES_color (RID_PLOT_MMAS_MAX), RES_color (RID_PLOT_BG), 10, 10, 2), ERR_GUI_PLOT_INIT_FAILED, "set.max.symbol");
	GUIERR (S, ERR_GUI_PLOT_INIT_FAILED);
	GUINEW (C, PlotCurve ("maximum", &(d.set.max), PLOT_LINE, RES_color (RID_PLOT_MMAS_MAX), 255, 2), ERR_GUI_PLOT_INIT_FAILED, "set.max.curve");
	GUIERR (C, ERR_GUI_PLOT_INIT_FAILED);
	C->setSymbol (*S); C->setZ (Z_MAX); C->attach (this);
	d.set.max.symbol = S; d.set.max.curve = C; d.set.max.col = MDLMMAS_COLUMN_MAXID;
	enable_curve (d.set.max.curve, true);

	/* min symbol and curve */
	GUINEW (S, PlotSymbol (QwtSymbol::Ellipse, RES_color (RID_PLOT_MMAS_MIN), RES_color (RID_PLOT_BG), 10, 10, 2), ERR_GUI_PLOT_INIT_FAILED, "set.min.symbol");
	GUIERR (S, ERR_GUI_PLOT_INIT_FAILED);
	GUINEW (C, PlotCurve ("minimum", &(d.set.min), PLOT_LINE, RES_color (RID_PLOT_MMAS_MIN), 255, 2), ERR_GUI_PLOT_INIT_FAILED, "set.min.curve");
	GUIERR (C, ERR_GUI_PLOT_INIT_FAILED);
	C->setSymbol (*S); C->setZ (Z_MIN); C->attach (this);
	d.set.min.symbol = S; d.set.min.curve = C; d.set.min.col = MDLMMAS_COLUMN_MINID;
	enable_curve (d.set.min.curve, true);

	/* avg symbol and curve */
	GUINEW (S, PlotSymbol (QwtSymbol::Ellipse, RES_color (RID_PLOT_MMAS_AVG), RES_color (RID_PLOT_BG), 10, 10, 2), ERR_GUI_PLOT_INIT_FAILED, "set.avg.symbol");
	GUIERR (S, ERR_GUI_PLOT_INIT_FAILED);
	GUINEW (C, PlotCurve ("average", &(d.set.avg), PLOT_LINE, RES_color (RID_PLOT_MMAS_AVG), 255, 2), ERR_GUI_PLOT_INIT_FAILED, "set.avg.curve");
	GUIERR (C, ERR_GUI_PLOT_INIT_FAILED);
	C->setSymbol (*S); C->setZ (Z_AVG); C->attach (this);
	d.set.avg.symbol = S; d.set.avg.curve = C; d.set.avg.col = MDLMMAS_COLUMN_NONE;
	enable_curve (d.set.avg.curve, true);

	/* spr symbol and curve */
	GUINEW (S, PlotSymbol (QwtSymbol::Ellipse, RES_color (RID_PLOT_MMAS_SPR), RES_color (RID_PLOT_BG), 10, 10, 2), ERR_GUI_PLOT_INIT_FAILED, "set.spr.symbol");
	GUIERR (S, ERR_GUI_PLOT_INIT_FAILED);
	GUINEW (C, PlotCurve ("spread", &(d.set.spr), PLOT_AREA, RES_color (RID_PLOT_MMAS_SPR), 64), ERR_GUI_PLOT_INIT_FAILED, "set.spr.curve");
	GUIERR (C, ERR_GUI_PLOT_INIT_FAILED);
	C->setSymbol (*S); C->setZ (Z_SPR); C->attach (this);
	d.set.spr.symbol = S; d.set.spr.curve = C; d.set.spr.col = MDLMMAS_COLUMN_NONE;
	enable_curve (d.set.spr.curve, true);

	/* cnt symbol and curve */
	GUINEW (S, PlotSymbol (QwtSymbol::Ellipse, RES_color (RID_PLOT_MMAS_CNT), RES_color (RID_PLOT_BG), 10, 10, 2), ERR_GUI_PLOT_INIT_FAILED, "set.cnt.symbol");
	GUIERR (S, ERR_GUI_PLOT_INIT_FAILED);
	GUINEW (C, PlotCurve ("count", &(d.set.cnt), PLOT_AREA, RES_color (RID_PLOT_MMAS_CNT), 64), ERR_GUI_PLOT_INIT_FAILED, "set.cnt.curve");
	GUIERR (C, ERR_GUI_PLOT_INIT_FAILED);
	C->setSymbol (*S); C->setZ (Z_CNT); C->attach (this);
	d.set.cnt.symbol = S; d.set.cnt.curve = C; d.set.cnt.col = MDLMMAS_COLUMN_NONE;
	enable_curve (d.set.cnt.curve, false);

	/* default time axis */
	d.axis_type = AXIS_TIME; set_scaledraw (d.axis_type);
	setAxisScale (QwtPlot::xBottom, 0, 1);
	//setAxisLabelRotation (QwtPlot::xBottom, -45.0);
	//setAxisLabelAlignment (QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
	setAxisLabelRotation (QwtPlot::xBottom, 0.0);
	setAxisLabelAlignment (QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom);

	sw = axisWidget (QwtPlot::xBottom);
	sw->setMinBorderDist (0, QFontMetrics (sw->font()).height() / 2);

	/* attribute axis */
	setAxisScale (QwtPlot::yLeft, 0, 1);

	sw = axisWidget (QwtPlot::yLeft);
	sd = axisScaleDraw (QwtPlot::yLeft);
	sd->setMinimumExtent (sd->extent (QPen(), sw->font()));

	/* Mark */
	GUINEW (d.mark, PlotMark (M_SPACE), ERR_GUI_PLOT_INIT_FAILED, "mark");
	GUIERR (d.mark, ERR_GUI_PLOT_INIT_FAILED);
	d.mark->attach (this);
	d.mark->hide();

	/* tracking */
	canvas()->setMouseTracking (true);
	canvas()->installEventFilter(this);

	/* plot */
	setTitle ("Minimum, Maximum, Average & Spread");
	plotLayout()->setAlignCanvasToScales (true);
	setAutoReplot (false);

	if (!connect (this, SIGNAL (legendChecked (QwtPlotItem *, bool)), SLOT (show (QwtPlotItem *, bool))))
		SET_GUICLS_ERROR (ERR_GUI_PLOT_INIT_FAILED, "failed to connect <legendChecked> to <show>");

	d.model = model;

	SET_GUICLS_NOERR;
}

PlotMMAS::~PlotMMAS (void)
{
	DBG_TRACE_DESTRUCT;

	delete d.p_bg;
	delete d.p_timeline;
	delete d.p_idxline;
	delete d.p_grid;
	delete d.p_legend;
	delete d.set.max.symbol;
	delete d.set.max.curve;
	delete d.set.min.symbol;
	delete d.set.min.curve;
	delete d.set.avg.symbol;
	delete d.set.avg.curve;
	delete d.set.spr.symbol;
	delete d.set.spr.curve;
	delete d.set.cnt.symbol;
	delete d.set.cnt.curve;
	delete d.mark;

	clear();
}

bool
PlotMMAS::eventFilter (QObject *obj, QEvent *e)
{
	if (obj != (QObject *)canvas()) return false;

	switch (e->type ()) {
		case QEvent::MouseMove:
			track (((QMouseEvent *) e)->pos ());
			return true;

		default:
			break;
	}

	return (QObject::eventFilter (obj, e));
}

void
PlotMMAS::refresh (void)
{
	DBG_TRACE_FENTER;

	if (!d.model) goto leave;

	clear();

	setTitle (d.model->title());

	if (!(d.set.count = d.model->rowCount())) goto leave;

	SL_SAFE_CALLOC (d.set.time, d.set.count, sizeof (double));
	SL_SAFE_CALLOC (d.set.axis, d.set.count, sizeof (double));
	SL_SAFE_CALLOC (d.set.min.data, d.set.count, sizeof (double));
	SL_SAFE_CALLOC (d.set.max.data, d.set.count, sizeof (double));
	SL_SAFE_CALLOC (d.set.avg.data, d.set.count, sizeof (double));
	SL_SAFE_CALLOC (d.set.spr.data, d.set.count, sizeof (double));
	SL_SAFE_CALLOC (d.set.cnt.data, d.set.count, sizeof (double));

	d.model->column (MDLMMAS_COLUMN_DATE, d.set.time, d.set.count);
	switch (d.axis_type) {
		case AXIS_INDEX:
			for (int i=0; i<d.set.count; i++) d.set.axis[i] = (double)i;
			break;
		case AXIS_TIME:
		default:
			memcpy (d.set.axis, d.set.time, d.set.count * sizeof (double));
			break;
	}

	d.model->column (MDLMMAS_COLUMN_MAX, d.set.max.data, d.set.count);
	d.model->column (MDLMMAS_COLUMN_MIN, d.set.min.data, d.set.count);
	d.model->column (MDLMMAS_COLUMN_AVG, d.set.avg.data, d.set.count);
	d.model->column (MDLMMAS_COLUMN_SPR, d.set.spr.data, d.set.count);
	d.model->column (MDLMMAS_COLUMN_COUNT, d.set.cnt.data, d.set.count);

	enable_curve (d.set.cnt.curve, d.model->filtered());

	update();
	replot();

leave:
	DBG_TRACE_FLEAVE;
}

void
PlotMMAS::set_axistype (AXIS_TYPE type)
{
	if (d.axis_type == type) return;

	d.axis_type = type;
	set_scaledraw (d.axis_type);
}

void
PlotMMAS::clear (void)
{
	if (d.set.time) SL_SAFE_FREE (d.set.time);
	if (d.set.axis) SL_SAFE_FREE (d.set.axis);
	if (d.set.max.data) SL_SAFE_FREE (d.set.max.data);
	if (d.set.min.data) SL_SAFE_FREE (d.set.min.data);
	if (d.set.avg.data) SL_SAFE_FREE (d.set.avg.data);
	if (d.set.spr.data) SL_SAFE_FREE (d.set.spr.data);
	if (d.set.cnt.data) SL_SAFE_FREE (d.set.cnt.data);
	d.set.count = 0;
}

void
PlotMMAS::update (void)
{
	int	i;
	double	xmin = 0.0, xmax = 0.0, ymax = 0.0;
	double	xadd, sdiv;

	for (i=0; i<d.set.count; i++) {
		if (i == 0) {
			xmin = xmax = d.set.axis[i];
		} else {
			if (d.set.axis[i] < xmin) xmin = d.set.axis[i];
			if (d.set.axis[i] > xmax) xmax = d.set.axis[i];
		}

		if ((i == 0) || (d.set.max.data[i] > ymax)) ymax = d.set.max.data[i];
	}

	switch (d.axis_type) {
		case AXIS_INDEX:
			sdiv = xadd = 1.0;
			break;
		case AXIS_TIME:
		default:
			xadd = (xmax - xmin); sdiv = (xadd >= T_DAY) ? 0.0 : S_SMALL;
			xadd *= 0.05; if (xadd == 0.0) xadd = S_SMALL;
			break;
	}

	if (fabs(ymax) > DOUBLEPREC) {
		ymax = ceil (ymax * 1.1);
		int m = (int) ymax % Y_ALIGN;
		if (m) ymax += (Y_ALIGN - m);
	} else {
		ymax = Y_ALIGN;
	}

	setAxisScale (QwtPlot::xBottom, xmin - xadd, xmax + xadd, sdiv ? sdiv : T_QUARTER);
	setAxisScale (QwtPlot::yLeft, 0, ymax);

	d.set.max.curve->setRawData (d.set.axis, d.set.max.data, d.set.count);
	d.set.min.curve->setRawData (d.set.axis, d.set.min.data, d.set.count);
	d.set.avg.curve->setRawData (d.set.axis, d.set.avg.data, d.set.count);
	d.set.spr.curve->setRawData (d.set.axis, d.set.spr.data, d.set.count);
	d.set.cnt.curve->setRawData (d.set.axis, d.set.cnt.data, d.set.count);
}

void
PlotMMAS::track (const QPoint &pos)
{
	QwtPlotItemIterator	it;
	QwtPlotCurve		*c;
	int			i = -1;
	double			dv;
	PlotMMASCurve		*cc, *curve = NULL;
	int			index = -1;
	double			dist = 10e10;
	SPWAW_SNAP_OOB_PTR	p;

	for (it = itemList().begin(); it != itemList().end(); ++it)
	{
		if ((*it)->rtti() != QwtPlotItem::Rtti_PlotCurve) continue;

		c = (QwtPlotCurve *)(*it);
		if (!c->isVisible()) continue;

		i = c->closestPoint (pos, &dv);
		// cppcheck-suppress cstyleCast
		cc = (PlotMMASCurve *)(((PlotCurve *)c)->parent());
		if ((dv < dist) || ((dv == dist) && (cc->col != MDLMMAS_COLUMN_NONE))) {
			// cppcheck-suppress cstyleCast
			curve = (PlotMMASCurve *)(((PlotCurve *)c)->parent());
			index = i;
			dist = dv;
		}
	}

	d.mark->hide();

	if (curve && (i >= 0) && dist <= M_TOL)
	{
		double vx = curve->curve->x(index);
		double vy = curve->curve->y(index);

		QRect r = plotLayout()->canvasRect();
		Qt::Alignment a = 0;
		if (pos.x() < (r.width()/2)) {
			a |= Qt::AlignRight;
		} else {
			a |= Qt::AlignLeft;
		}
		if (pos.y() < (r.height()/2)) {
			a |= Qt::AlignBottom;
		} else {
			a |= Qt::AlignTop;
		}

		d.mark->setValue (vx, vy);
		d.mark->setAlign (a);

		QString s;

		p.up = d.model->unitid (curve->col, index);
		if (p.up) {
			s.sprintf ("%d (%s: %s)", (int)vy, p.up->strings.uid, p.up->data.type);
		} else {
			s.sprintf ("%.2f", vy);
		}

		d.mark->setText (s);
		d.mark->show();
	}
	replot ();
}

void
PlotMMAS::set_scaledraw (AXIS_TYPE type)
{
	QwtScaleDraw	*p = NULL;

	switch (type) {
		case AXIS_TIME:
		default:
			p = new PlotTimeline (*(d.p_timeline));
			break;
		case AXIS_INDEX:
			p = new PlotIdxline (*(d.p_idxline));
			break;
	}
	setAxisScaleDraw (QwtPlot::xBottom, p);
}

void
PlotMMAS::enable_curve (QwtPlotItem *item, bool on)
{
	QWidget	*w = NULL;

	item->setVisible (on);
	w = legend()->find (item);

	if (w && w->inherits ("QwtLegendItem")) ((QwtLegendItem *)w)->setChecked (on);
}

void
PlotMMAS::show (QwtPlotItem *item, bool on)
{
	enable_curve (item, on);
	replot();
}
