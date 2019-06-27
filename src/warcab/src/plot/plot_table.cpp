/** \file
 * The SPWaW war cabinet - plot handling - table plot handling.
 *
 * Copyright (C) 2005-2019 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_table.h"
#include "plot_background.h"
#include "plot_grid.h"
#include "plot_timeline.h"
#include "plot_idxline.h"
#include "plot_legend.h"
#include "plot_symbol.h"
#include "plot_curve.h"
#include "plot_bar.h"
#include "plot_mark.h"
#include "plot_private.h"

#define	Z_START		PLOT_Z_USER

#define	T_DAY		(1.0)
#define	T_QUARTER	(T_DAY * 31 * 3)
#define	S_SMALL		(1.0/24.0/12.0)

#define	Y_ALIGN		20

#define	M_SPACE		10

#define	M_TOL_LINE	(25.0)
#define	M_TOL_BAR	(1.0e6)

#define	DEF_WIDTH	(T_DAY * 20)

/* Convenience macro */
#define	DOUBLEPREC	1.0e-6

PlotTable::PlotTable (ModelPlotTable *model, QWidget *P)
	: QwtPlot (P)
{
	QwtScaleWidget	*sw = NULL;
	QwtScaleDraw	*sd = NULL;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	if (!model) SET_GUICLS_ERROR (ERR_GUI_PLOT_INIT_FAILED, "no model provided");
	d.model = model;

	/* background */
	GUINEW (d.p_bg, PlotBG(), ERR_GUI_PLOT_INIT_FAILED, "p_bg");
	GUIERR (d.p_bg, ERR_GUI_PLOT_INIT_FAILED);
	d.p_bg->attach (this);

	/* scale drawer for time axis */
	GUINEW (d.p_timeline, PlotTimeline(), ERR_GUI_PLOT_INIT_FAILED, "p_timeline");
	GUIERR (d.p_timeline, ERR_GUI_PLOT_INIT_FAILED);

	/* scale drawer for index axis */
	GUINEW (d.p_idxline, PlotIdxline(&(d.set.row_cnt), &(d.set.time)), ERR_GUI_PLOT_INIT_FAILED, "p_idxline");
	GUIERR (d.p_idxline, ERR_GUI_PLOT_INIT_FAILED);

	/* grid */
	GUINEW (d.p_grid, PlotGrid(), ERR_GUI_PLOT_INIT_FAILED, "p_grid");
	GUIERR (d.p_grid, ERR_GUI_PLOT_INIT_FAILED);
	d.p_grid->attach (this);

	/* legend */
	GUINEW (d.p_legend, PlotLegend (true), ERR_GUI_PLOT_INIT_FAILED, "p_legend");
	GUIERR (d.p_legend, ERR_GUI_PLOT_INIT_FAILED);
	//d.p_legend->setItemMode (QwtLegend::ReadOnlyItem);
	insertLegend (d.p_legend, QwtPlot::RightLegend);

	/* use time axis by default */
	d.axis_type = AXIS_TIME; set_scaledraw (d.axis_type);
	setAxisScale (QwtPlot::xBottom, 0, 1);
	//setAxisLabelRotation (QwtPlot::xBottom, -45.0);
	//setAxisLabelAlignment (QwtPlot::xBottom, Qt::AlignLeft | Qt::AlignBottom);
	setAxisLabelRotation (QwtPlot::xBottom, 0.0);
	setAxisLabelAlignment (QwtPlot::xBottom, Qt::AlignHCenter | Qt::AlignBottom);

	sw = axisWidget (QwtPlot::xBottom);
	sw->setMinBorderDist (0, QFontMetrics (sw->font()).height() / 2);

	/* value axis */
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
	setTitle ("Table Plot");
	plotLayout()->setAlignCanvasToScales (true);
	setAutoReplot (false);

	if (!connect (this, SIGNAL (legendChecked (QwtPlotItem *, bool)), SLOT (show (QwtPlotItem *, bool))))
		SET_GUICLS_ERROR (ERR_GUI_PLOT_INIT_FAILED, "failed to connect <legendChecked> to <show>");

	SET_GUICLS_NOERR;
}

PlotTable::~PlotTable (void)
{
	DBG_TRACE_DESTRUCT;

	clear ();

	delete d.p_bg;
	delete d.p_timeline;
	delete d.p_idxline;
	delete d.p_grid;
	delete d.p_legend;
	delete d.mark;
}

bool
PlotTable::eventFilter (QObject *obj, QEvent *e)
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
PlotTable::setup_def (void)
{
	int		cnt, i, av;
	PlotCurve	*C = NULL;
	PlotBar		*B = NULL;
	PlotSymbol	*S = NULL;
	bool		error = false;

	if (!d.model || !(d.cfg = d.model->def_info())) return;
	clear();

	d.set.stacked = d.cfg->plot_stacked;
	setTitle (d.cfg->title);

	set_axistype (d.cfg->axis_type);

	cnt = d.cfg->col_cnt; if (cnt <= 1) return;
	d.set.curve_cnt = cnt-1; SL_SAFE_CALLOC (d.set.curve, d.set.curve_cnt, sizeof (PlotTableCurve));

	/* Create curve for each column except the first */
	for (i=0; i<d.set.curve_cnt; i++) {
		d.set.curve[i].set = &(d.set);
		d.set.curve[i].idx = i;

		MDLPT_COLDEF *col = &(d.cfg->col_lst[i+1]);

		switch (d.cfg->plot_type) {
			case PLOT_LINE:
			case PLOT_AREA:
			default:
				/* symbol */
				// TODO: optional style, brushcolor, xsize, ysize, width
				S = new PlotSymbol (QwtSymbol::Ellipse, col->plot_color, RES_color (RID_PLOT_BG), 10, 10, 2);
				if (!S) { error = true; break; }
				d.set.curve[i].symbol = S;

				/* curve */
				d.set.curve[i].type = PLOTTABLE_CURVE;

				// TODO: optional type, alpha, width
				av = ((d.cfg->plot_type == PLOT_AREA) && !d.set.stacked) ? 128 : 255;
				C = new PlotCurve (col->name, &(d.set.curve[i]), d.cfg->plot_type, col->plot_color, av, 2);
				if (!C) { error = true; break; }
				C->setSymbol (*S); C->setZ (Z_START + d.set.curve_cnt - i); C->attach (this);
				d.set.curve[i].u.curve = C;
				d.set.curve[i].u.curve->setRawData (d.set.axis, d.set.curve[i].data, 0);
				break;
			case PLOT_BAR:
				/* symbol */
				// TODO: optional style, brushcolor, xsize, ysize, width
				S = new PlotSymbol (QwtSymbol::Rect, col->plot_color, col->plot_color, 10, 10, 2);
				if (!S) { error = true; break; }
				d.set.curve[i].symbol = S;

				/* bar */
				d.set.curve[i].type = PLOTTABLE_BAR;

				// TODO: optional type, alpha, width
				av = (!d.set.stacked) ? 128 : 255;
				//av = (!d.set.stacked) ? 255 : 128;
				B = new PlotBar (col->name, &(d.set.curve[i]), d.cfg->plot_type, col->plot_color, av, 2);
				if (!B) { error = true; break; }
				B->setSymbol (*S); B->setZ (Z_START + d.set.curve_cnt - i); B->attach (this);
				d.set.curve[i].u.bar = B;
				d.set.curve[i].u.bar->setRawData (d.set.axis, d.set.curve[i].data, 0);
				break;
		}
	}
	if (error) clear();
}

void
PlotTable::setup_dat (void)
{
	int	i, j;

	if (!d.model) return;
	clear_dat ();

	if (!(d.set.row_cnt = d.model->rowCount())) return;

	SL_SAFE_CALLOC (d.set.time, d.set.row_cnt, sizeof (double));
	d.model->plot_data (0, d.set.time, d.set.row_cnt);

	SL_SAFE_CALLOC (d.set.axis, d.set.row_cnt, sizeof (double));
	switch (d.cfg->axis_type) {
		case AXIS_INDEX:
			for (i=0; i<d.set.row_cnt; i++) d.set.axis[i] = (double)i;
			break;
		case AXIS_TIME:
		default:
			memcpy (d.set.axis, d.set.time, d.set.row_cnt * sizeof (double));
			break;
	}

	for (i=0; i<d.set.curve_cnt; i++) {
		SL_SAFE_CALLOC (d.set.curve[i].data, d.set.row_cnt, sizeof (double));
		d.model->plot_data (i+1, d.set.curve[i].data, d.set.row_cnt);
		SL_SAFE_CALLOC (d.set.curve[i].reld, d.set.row_cnt, sizeof (double));
	}

	if (d.set.stacked) {
		for (i=0; i<d.set.row_cnt; i++) {
			for (j=1; j<d.set.curve_cnt; j++) {
				d.set.curve[j].data[i] += d.set.curve[j-1].data[i];
			}
			// TODO: consider making this optional? (cfr stacked vs 100% stacked in excel)
			for (j=0; j<d.set.curve_cnt; j++) {
				d.set.curve[j].data[i] = d.set.curve[j].data[i] / d.set.curve[d.set.curve_cnt-1].data[i] * 100.0;
			}
			d.set.curve[0].reld[i] = d.set.curve[0].data[i];
			for (j=1; j<d.set.curve_cnt; j++) {
				d.set.curve[j].reld[i] = d.set.curve[j].data[i] - d.set.curve[j-1].data[i];
			}
		}
	}
}

void
PlotTable::clear (void)
{
	clear_dat ();
	clear_def ();

	if (d.set.curve) SL_SAFE_FREE (d.set.curve);
	d.set.curve_cnt = 0;
}

void
PlotTable::clear_def (void)
{
	int	i;

	for (i=0; i<d.set.curve_cnt; i++) {
		curve_delete (i);
		delete d.set.curve[i].symbol;
	}
}

void
PlotTable::clear_dat (void)
{
	int	i;

	if (d.set.time) SL_SAFE_FREE (d.set.time);
	if (d.set.axis) SL_SAFE_FREE (d.set.axis);
	for (i=0; i<d.set.curve_cnt; i++) {
		if (d.set.curve[i].data) SL_SAFE_FREE (d.set.curve[i].data);
		if (d.set.curve[i].reld) SL_SAFE_FREE (d.set.curve[i].reld);
	}
	d.set.row_cnt = 0;
}

void
PlotTable::plot (void)
{
	int	i, j, m;
	double	xmin = 0.0, xmax = 0.0, ymax = 0.0, ytot;
	double	xadd, sdiv;
	double	minw, w;

	if (!d.set.curve_cnt) return;

	if (!d.set.row_cnt) {
		for (i=0; i<d.set.curve_cnt; i++) {
			curve_setdata (i, NULL, NULL, 0);
			d.set.curve[i].empty = true;
		}
	} else {
		for (i=0; i<d.set.row_cnt; i++) {
			if (i == 0) {
				xmin = xmax = d.set.axis[i];
			} else {
				if (d.set.axis[i] < xmin) xmin = d.set.axis[i];
				if (d.set.axis[i] > xmax) xmax = d.set.axis[i];
			}
			for (j=0; j<d.set.curve_cnt; j++) {
				if (((i+j) == 0) || (d.set.curve[j].data[i] > ymax)) ymax = d.set.curve[j].data[i];
			}
		}

		switch (d.cfg->axis_type) {
			case AXIS_INDEX:
				sdiv = xadd = 1.0;
				break;
			case AXIS_TIME:
			default:
				xadd = (xmax - xmin); sdiv = (xadd >= T_DAY) ? 0.0 : S_SMALL;
				xadd *= 0.05; if (xadd == 0.0) xadd = S_SMALL;
				break;
		}

		ymax = ceil (ymax * 1.1); m = (int) ymax % Y_ALIGN;
		if (m) ymax += (Y_ALIGN - m);

		setAxisScale (QwtPlot::xBottom, xmin - xadd, xmax + xadd, sdiv ? sdiv : T_QUARTER);
		setAxisScale (QwtPlot::yLeft, 0, ymax);

		for (i=0; i<d.set.curve_cnt; i++) {
			curve_setdata (i, d.set.axis, d.set.curve[i].data, d.set.row_cnt);
			if (sdiv) {
				if (!d.set.stacked && (d.set.curve_cnt > 1)) {
					minw = sdiv * 0.10;
					w = (sdiv * 0.8 / (double)(d.set.curve_cnt)) - 0.025;
					if (w < minw) w = minw;
					curve_setwidth (i, w);
					curve_setdodge (i, (((double)(i*2 + 1) / (double)(d.set.curve_cnt*2)) - 0.5) * sdiv * 0.8);
				} else {
					curve_setwidth (i, sdiv * 0.5);
				}
			} else {
				curve_setwidth (i, DEF_WIDTH);
			}

			d.set.curve[i].empty = false; ytot = 0.0;
			for (j=0; j<d.set.row_cnt; j++) {
				if (!d.set.stacked) {
					ytot += fabs(d.set.curve[i].data[j]);
				} else {
					ytot += fabs(d.set.curve[i].reld[j]);
				}
			}
			d.set.curve[i].empty = (fabs(ytot) <= DOUBLEPREC);
		}
	}
	for (i=0; i<d.set.curve_cnt; i++) curve_show (i);
	replot();
}

void
PlotTable::track (const QPoint &pos)
{
	QwtPlotItemIterator	it;
	QwtPlotCurve		*c = NULL;
	PlotBarItem		*b = NULL;
	int			i = -1;
	double			dv = -1.0;
	double			zv = 0.0;
	PlotTableCurve		*p = NULL, *curve = NULL;
	int			index = -1;
	double			dist = 10e10;
	double			maxz = 0.0;
	double			vx, vy, tvx, tvy, dy;

	for (it = itemList().begin(); it != itemList().end(); ++it)
	{
		if (!(*it)->isVisible()) continue;
		if (	((*it)->rtti() != QwtPlotItem::Rtti_PlotCurve)		&&
			((*it)->rtti() != QwtPlotItem::Rtti_PlotUserItem)	)
		{
			continue;
		}

		if ((*it)->rtti() == QwtPlotItem::Rtti_PlotCurve) {
			c = (QwtPlotCurve *)(*it);
			if (!c->dataSize()) continue;
			// cppcheck-suppress cstyleCast
			p = (PlotTableCurve *)(((PlotCurve *)c)->parent());
			i = c->closestPoint (pos, &dv);
			zv = c->z();
		}
		if ((*it)->rtti() == QwtPlotItem::Rtti_PlotUserItem) {
			b = (PlotBarItem *)(*it);
			if (!b->dataSize()) continue;
			// cppcheck-suppress cstyleCast
			p = (PlotTableCurve *)(((PlotBar *)b)->parent());
			i = b->closestPoint (pos, &dv);
			zv = b->z();
		}

		if ((i >= 0)&& (dv >= 0.0) && (dv <= dist) && (zv > maxz)) {
			curve = p; index = i; dist = dv; maxz = zv;
		}
	}

	d.mark->hide();

	if (curve && (dist <= curve_tol (curve)))
	{
		vx = curve_vx (curve, index);
		vy = curve_vy (curve, index);

		if (!curve->set->stacked)
			dy = vy;
		else
			dy = curve->reld[index];

		tvx = canvasMap(QwtPlot::xBottom).xTransform(vx);
		tvy = canvasMap(QwtPlot::yLeft).xTransform(vy);

		QRect r = plotLayout()->canvasRect();

		Qt::Alignment a = 0;
		switch (curve->type) {
			case PLOTTABLE_CURVE:
			default:
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
				break;
			case PLOTTABLE_BAR:
				if (tvx < (r.width()/2)) {
					a |= Qt::AlignRight;
				} else {
					a |= Qt::AlignLeft;
				}
				if (tvy < (r.height()/2)) {
					a |= Qt::AlignBottom;
				} else {
					a |= Qt::AlignTop;
				}
				break;
		}

		d.mark->setValue (vx, vy);
		d.mark->setAlign (a);

		QString s;
		s.sprintf ("%s: %.2f", curve_name (curve), dy);
		if (curve->set->stacked) s += " %";

		d.mark->setText (s);
		d.mark->show();
	}
	replot ();
}

void
PlotTable::set_axistype (AXIS_TYPE type)
{
	d.axis_type = type;
	set_scaledraw (d.axis_type);
}

void
PlotTable::set_scaledraw (AXIS_TYPE type)
{
	QwtScaleDraw	*p = NULL;

	switch (type) {
		case AXIS_TIME:
		default:
			p = new PlotTimeline (*(d.p_timeline));
			//DBG_log ("%s - 0x%8.8x - new PlotTimeline @0x%8.8x", __FUNCTION__, this, p);
			break;
		case AXIS_INDEX:
			p = new PlotIdxline (*(d.p_idxline));
			//DBG_log ("%s - 0x%8.8x - new PlotIdxline @0x%8.8x", __FUNCTION__, this, p);
			break;
	}
	setAxisScaleDraw (QwtPlot::xBottom, p);
}

void
PlotTable::commit (void)
{
	setup_def();
	setup_dat();
	plot();
}

void
PlotTable::commit_def (void)
{
	setup_def();
	plot();
}

void
PlotTable::commit_dat (void)
{
	setup_dat();
	plot();
}

void
PlotTable::curve_setdata (int idx, const double *x, const double *y, int size)
{
	switch (d.set.curve[idx].type) {
		case PLOTTABLE_CURVE:
			d.set.curve[idx].u.curve->setRawData (x, y, size);
			break;
		case PLOTTABLE_BAR:
			d.set.curve[idx].u.bar->setRawData (x, y, size);
			break;
		default:
			break;
	}
}

void
PlotTable::curve_setdodge (int idx, double dodge)
{
	switch (d.set.curve[idx].type) {
		case PLOTTABLE_BAR:
			d.set.curve[idx].u.bar->setDodge (dodge);
			break;
		case PLOTTABLE_CURVE:
		default:
			break;
	}
}

void
PlotTable::curve_setwidth (int idx, double width)
{
	switch (d.set.curve[idx].type) {
		case PLOTTABLE_BAR:
			d.set.curve[idx].u.bar->setWidth (width);
			break;
		case PLOTTABLE_CURVE:
		default:
			break;
	}
}

void
PlotTable::curve_show (int idx)
{
	switch (d.set.curve[idx].type) {
		case PLOTTABLE_CURVE:
			enable_curve (d.set.curve[idx].u.curve, !d.set.curve[idx].empty);
			break;
		case PLOTTABLE_BAR:
			enable_curve (d.set.curve[idx].u.bar, !d.set.curve[idx].empty);
			break;
		default:
			break;
	}
}

void
PlotTable::curve_delete (int idx)
{
	switch (d.set.curve[idx].type) {
		case PLOTTABLE_CURVE:
			enable_curve (d.set.curve[idx].u.curve, false);
			d.set.curve[idx].u.curve->detach();
			delete d.set.curve[idx].u.curve;
			break;
		case PLOTTABLE_BAR:
			enable_curve (d.set.curve[idx].u.bar, false);
			d.set.curve[idx].u.bar->detach();
			delete d.set.curve[idx].u.bar;
			break;
		default:
			break;
	}
}

double
PlotTable::curve_vx (PlotTableCurve *curve, int idx)
{
	double	v = 0.0;

	if (!curve) return (v);

	switch (curve->type) {
		case PLOTTABLE_CURVE:
			v = curve->u.curve->x(idx);
			break;
		case PLOTTABLE_BAR:
			v = curve->u.bar->x(idx);
			break;
		default:
			break;
	}
	return (v);
}

double
PlotTable::curve_vy (PlotTableCurve *curve, int idx)
{
	double	v = 0.0;

	if (!curve) return (v);

	switch (curve->type) {
		case PLOTTABLE_CURVE:
			v = curve->u.curve->y(idx);
			break;
		case PLOTTABLE_BAR:
			v = curve->u.bar->y(idx);
			break;
		default:
			break;
	}
	return (v);
}

double
PlotTable::curve_tol (PlotTableCurve *curve)
{
	double	v = 0.0;

	if (!curve) return (v);

	switch (curve->type) {
		case PLOTTABLE_CURVE:
			v = M_TOL_LINE;
			break;
		case PLOTTABLE_BAR:
			v = M_TOL_BAR;
			break;
		default:
			break;
	}
	return (v);
}

char *
PlotTable::curve_name (PlotTableCurve *curve)
{
	char	*v = NULL;

	if (!curve) return (v);

	switch (curve->type) {
		case PLOTTABLE_CURVE:
			v = curve->u.curve->name();
			break;
		case PLOTTABLE_BAR:
			v = curve->u.bar->name();
			break;
		default:
			break;
	}
	return (v);
}

void
PlotTable::enable_curve (QwtPlotItem *item, bool on)
{
	QWidget	*w = NULL;

	item->setVisible (on);
	w = legend()->find (item);

	if (w && w->inherits ("QwtLegendItem")) ((QwtLegendItem *)w)->setChecked (on);
}

void
PlotTable::show (QwtPlotItem *item, bool on)
{
	enable_curve (item, on);
	replot();
}
