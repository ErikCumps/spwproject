/** \file
 * The SPWaW war cabinet - plot handling - bar-type plot.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "plot_bar_item.h"

// Helper object to filter the paint event and repeat the PlotBarItem::draw inside the paint event
typedef class PlotBarItem PBI;
class PBIPH	: public QObject
{
public:
	PBIPH	(const PBI *i, int r0, int r1)
		:i(i),r0(r0),r1(r1) {}

public:
	virtual	bool	eventFilter	(QObject *, QEvent *e)
	{
		bool	b = (e->type() == QEvent::Paint);
		if (b) i->draw (r0, r1);
		return (b);
	}

private:
	const PBI	*i;
	int		r0;
	int		r1;
};

// From QwtPlotCurve:
//
// Creating and initializing a QPainter is an expensive operation. So we keep a
// painter open for situations, where we paint outside of paint events. This
// improves the performance of incremental painting a lot.
// But it is not possible to have more than one QPainter open at the same time.
// So we need to close it before regular paint events are processed.
class PBIGP	: public QObject
{
public:
	~PBIGP	(void)
	{ end (); }

public:
	QPainter *	begin	(QwtPlotCanvas *canvas)
	{
		if ((cnv = canvas) != NULL) {
			QMap <QwtPlotCanvas *, QPainter *>::iterator it = map.find(cnv);
			if (it == map.end()) {
				QPainter *p = new QPainter (cnv);

				p->setClipping (true);
				p->setClipRect (cnv->contentsRect());
				it = map.insert (cnv, p);

				cnv->installEventFilter (this);
			}
			return it.value();
		} else {
			return (NULL);
		}
	}

	void		end	(void)
	{
		if (cnv) {
			QMap <QwtPlotCanvas *, QPainter *>::iterator it = map.find(cnv);
			if (it != map.end()) {
				cnv->removeEventFilter (this);
				delete it.value();
				map.erase (it);
			}
		}
	}

	virtual bool	eventFilter	(QObject *, QEvent *e)
	{
		if (e->type() == QEvent::Paint) end();
		return (false);
	}

private:
	QPointer <QwtPlotCanvas>			cnv;
	static QMap <QwtPlotCanvas *, QPainter *>	map;
};
QMap <QwtPlotCanvas *, QPainter *>PBIGP::map;

static int
verifyRange (int size, int &i1, int &i2)
{
	if (size < 1) return (0);

	i1 = qwtLim (i1, 0, size - 1);
	i2 = qwtLim (i2, 0, size - 1);
	if (i1 > i2) qSwap (i1, i2);

	return (i2 - i1 + 1);
}

class PlotBarItem::PrivateData
{

public:
	PrivateData	(void);
	~PrivateData	(void);

public:
	PlotBarItem::PlotType	type;
	double			base;
	QPen			pen;
	QBrush			brush;
	QwtSymbol		*symbol;
	double			dodge;
	double			width;
	PBIGP			gp;
	QRect			crect;

private:
	PrivateData(const PrivateData&); // these objects are non-copyable

};

PlotBarItem::PrivateData::PrivateData (void)
{
	type = Yfx;
	base = 0.0;
	pen = QPen (Qt::black);
	brush = QBrush (Qt::black);
	symbol = new QwtSymbol();
	dodge = 0.0;
	width = 0.5;
}

PlotBarItem::PrivateData::~PrivateData (void)
{
	delete (symbol);
}

PlotBarItem::PlotBarItem (void)
	: QwtPlotItem (QwtText ())
{
	init();
}

PlotBarItem::PlotBarItem (const QwtText &title)
	: QwtPlotItem (title)
{
	init();
}

PlotBarItem::PlotBarItem (const QString &title)
	: QwtPlotItem (QwtText(title))
{
	init();
}

PlotBarItem::~PlotBarItem (void)
{
	delete d_xy;
	delete d_data;
}

int
PlotBarItem::rtti (void) const
{
	return (QwtPlotItem::Rtti_PlotUserItem);
}

void
PlotBarItem::setPlotType (PlotType type)
{
	if (d_data->type != type) {
		d_data->type = type;
		itemChanged();
	}
}

PlotBarItem::PlotType
PlotBarItem::plotType (void) const
{
	return (d_data->type);
}

void
PlotBarItem::setBaseline (double base)
{
	if (d_data->base != base) {
		d_data->base = base;
		itemChanged();
	}
}

double
PlotBarItem::baseline (void) const
{
	return (d_data->base);
}

void
PlotBarItem::setPen (const QPen &pen)
{
	if (pen != d_data->pen) {
		d_data->pen = pen;
		itemChanged();
	}
}

const QPen &
PlotBarItem::pen (void) const
{
	return (d_data->pen);
}

void
PlotBarItem::setBrush (const QBrush &brush)
{
	if (brush != d_data->brush) {
		d_data->brush = brush;
		itemChanged();
	}
}

const QBrush &
PlotBarItem::brush (void) const
{
	return (d_data->brush);
}

void
PlotBarItem::setSymbol (const QwtSymbol &symbol)
{
	delete d_data->symbol;
	d_data->symbol = symbol.clone();
	itemChanged();
}

const QwtSymbol &
PlotBarItem::symbol (void) const
{
	return (*(d_data->symbol));
}

void
PlotBarItem::setDodge (double dodge)
{
	if (d_data->dodge != dodge) {
		d_data->dodge = dodge;
		itemChanged();
	}
}

double
PlotBarItem::dodge (void) const
{
	return (d_data->dodge);
}

void
PlotBarItem::setWidth (double width)
{
	if (d_data->width != width) {
		d_data->width = width;
		itemChanged();
	}
}

double
PlotBarItem::width (void) const
{
	return (d_data->width);
}

void
PlotBarItem::setRawData (const double *x, const double *y, int size)
{
	delete d_xy;

	d_xy = new QwtCPointerData (x, y, size);
	itemChanged();
}

void
PlotBarItem::setData (const double *x, const double *y, int size)
{
	delete d_xy;

	d_xy = new QwtArrayData (x, y, size);
	itemChanged();
}

void
PlotBarItem::setData (const QwtArray<double>&x, const QwtArray<double>&y)
{
	delete d_xy;

	d_xy = new QwtArrayData (x, y);
	itemChanged();
}

void
PlotBarItem::setData (const QPolygonF &data)
{
	delete d_xy;

	d_xy = new QwtPolygonFData (data);
	itemChanged();
}

void
PlotBarItem::setData (const QwtData &data)
{
	delete d_xy;

	d_xy = data.copy();
	itemChanged();
}

QwtData &
PlotBarItem::data (void)
{
	return (*d_xy);
}

const QwtData &
PlotBarItem::data (void) const
{
	return (*d_xy);
}

int
PlotBarItem::dataSize (void) const
{
	return (d_xy->size());
}

double
PlotBarItem::x (int i) const
{
	return (d_xy->x(i));
}

double
PlotBarItem::y (int i) const
{
	return (d_xy->y(i));
}

QwtDoubleRect
PlotBarItem::boundingRect (void) const
{
	if (!d_xy) {
		// return an invalid dounding rectangle
		return (QwtDoubleRect (1.0, 1.0, -2.0, -2.0));
	} else {
		return (d_xy->boundingRect());
	}
}

double
PlotBarItem::minXValue (void) const
{
	return (boundingRect().left());
}

double
PlotBarItem::maxXValue (void) const
{
	return (boundingRect().right());
}

double
PlotBarItem::minYValue (void) const
{
	return (boundingRect().top());
}

double
PlotBarItem::maxYValue (void) const
{
	return (boundingRect().bottom());
}

void
PlotBarItem::draw (QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, const QRect &r) const
{
	if (!p || (dataSize() <= 0)) return;

	d_data->crect = r;
	draw (p, xm, ym, 0, -1);
	d_data->crect = QRect();
}

void
PlotBarItem::draw (QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, int r0, int r1) const
{
	if (!p || (dataSize() <= 0)) return;

	if (r1 < 0) r1 = dataSize() - 1;
	if (verifyRange (dataSize(), r0, r1) <= 0) return;

	p->save ();
	p->setPen (QwtPainter::scaledPen (d_data->pen));
	p->setBrush (d_data->brush);
	drawBars (p, xm, ym, r0, r1);
	p->restore ();
}

void
PlotBarItem::draw (int r0, int r1) const
{
	QwtPlotCanvas	*cnv = NULL;
	QwtScaleMap	xm, ym;

	if (!plot() || !(cnv = plot()->canvas())) return;

	if (	!cnv->testAttribute (Qt::WA_WState_InPaintEvent)	&&
		!cnv->testAttribute (Qt::WA_PaintOutsidePaintEvent)	)
	{
		/* We save curve and range in helper and call repaint. The helper filters the Paint
		   event, to repeat the PlotBarItem::draw, but now from inside the paint event. */
		PBIPH	helper (this, r0, r1);
		cnv->installEventFilter (&helper);

		bool old_nsb = cnv->testAttribute (Qt::WA_NoSystemBackground);
		cnv->setAttribute (Qt::WA_NoSystemBackground, true);
		cnv->repaint ();
		cnv->setAttribute (Qt::WA_NoSystemBackground, old_nsb);
		return;
	}

	xm = plot()->canvasMap(xAxis());
	ym = plot()->canvasMap(yAxis());

	if (	cnv->testPaintAttribute (QwtPlotCanvas::PaintCached)	&&
		cnv->paintCache()					&&
		!cnv->paintCache()->isNull()				)
	{
		QPainter	p ((QPixmap *)cnv->paintCache());

		p.translate (-(cnv->contentsRect().x()), -(cnv->contentsRect().y()));
		draw (&p, xm, ym, r0, r1);
	}

	if (cnv->testAttribute (Qt::WA_WState_InPaintEvent)) {
		QPainter	p (cnv);

		p.setClipping (true);
		p.setClipRect (cnv->contentsRect());
		draw (&p, xm, ym, r0, r1);
	} else {
		QPainter	*p = d_data->gp.begin (cnv);

		draw (p, xm, ym, r0, r1);
	}
}

void
PlotBarItem::updateLegend (QwtLegend *l) const
{
	QWidget		*w = NULL;
	QwtLegendItem	*li = NULL;
	bool		old_du;
	int		plcy, mode;

	if (!l) return;

	QwtPlotItem::updateLegend (l);

	w = l->find (this);
	if (!w || !w->inherits ("QwtLegendItem")) return;

	li = (QwtLegendItem *)w;

	old_du = li->updatesEnabled();
	li->setUpdatesEnabled (false);

	plcy = l->displayPolicy ();
	if (plcy == QwtLegend::FixedIdentifier)
	{
		mode = l->identifierMode();
		if (mode & QwtLegendItem::ShowLine) {
			li->setCurvePen (pen());
		}
		if (mode & QwtLegendItem::ShowSymbol) {
			li->setSymbol (symbol());
		}
		if (mode & QwtLegendItem::ShowText) {
			li->setText (title());
		} else {
			li->setText (QwtText());
		}
		li->setIdentifierMode (mode);
	} else if (plcy == QwtLegend::AutoIdentifier)
	{
		mode = 0;

		mode |= QwtLegendItem::ShowLine;
		li->setCurvePen (pen());

		if (QwtSymbol::NoSymbol != symbol().style())
	        {
			li->setSymbol(symbol());
			mode |= QwtLegendItem::ShowSymbol;
		}

		if (!title().isEmpty()) {
			mode |= QwtLegendItem::ShowText;
			li->setText (title());
		} else {
			li->setText (QwtText());
		}
		li->setIdentifierMode (mode);
	}

	li->setUpdatesEnabled (old_du);
	li->update();
}

int
PlotBarItem::closestPoint (const QPoint &pos, double *dist) const
{
	int		index, i;
	QwtScaleMap	xm, ym;
	double		rx, ry, wx, wy, dx, dy;
	double		min;

	index = -1;
	if (dist) *dist = 1.0e10;

	if ((plot() == NULL) || (dataSize() <= 0)) return (-1);
	xm = plot()->canvasMap (xAxis()); ym = plot()->canvasMap (yAxis());

	rx = xm.xTransform (d_data->base);
	ry = ym.xTransform (d_data->base);

	wx = xm.xTransform (d_data->width) - rx;
	wy = ym.xTransform (d_data->width) - ry;

	if (d_data->type == Xfy) {
		min = wy / 2;
	} else {
		min = wx / 2;
	}

	for (i=0; i<dataSize(); i++)
	{
		double cx = xm.xTransform (x(i));
		double cy = ym.xTransform (y(i));

		if (d_data->type == Xfy) {
			if ((pos.x() < ry) || (cy < pos.y())) continue;

			cy += ym.xTransform (d_data->dodge) - ry;

			dy = fabs (cy - pos.y());
			dx = pos.x() - cx;

			if (dy <= min) {
				index = i; min = dy;
				if (dist) *dist = dx;
			}

		} else {
			if ((pos.y() < cy) || (ry < pos.y())) continue;

			cx += xm.xTransform (d_data->dodge) - rx;

			dx = fabs (cx - pos.x());
			dy = pos.y() - cy;

			if (dx <= min) {
				index = i; min = dx;
				if (dist) *dist = dy;
			}
		}
	}
	return (index);
}

void
PlotBarItem::init (void)
{
	setItemAttribute (QwtPlotItem::Legend);
	setItemAttribute (QwtPlotItem::AutoScale);

	d_data = new PrivateData;
	d_xy = new QwtPolygonFData (QwtArray<QwtDoublePoint>());

	setZ (20.0);
}

void
PlotBarItem::drawBars (QPainter *p, const QwtScaleMap &xm, const QwtScaleMap &ym, int r0, int r1) const
{
	QColor	c, lc, dc;
	int	i, x0, y0, xd, yd, xw, yw;
	QPoint	tl, tr, bl, br;
	QRect	bar;

	p->save();

	c  = p->brush().color();
	lc = c.light(125);
	dc = c.dark(125);

	x0 = xm.transform (d_data->base); y0 = ym.transform (d_data->base);

	xd = xm.transform (d_data->dodge) - x0;
	yd = ym.transform (d_data->dodge) - y0;

	xw = xm.transform (d_data->width) - x0;
	yw = ym.transform (d_data->width) - y0;

	for (i=r0; i<=r1; i++) {
		int xi = xm.transform (x(i));
		int yi = ym.transform (y(i));

		if (d_data->type == Xfy) {
			yi += yd;

			tl = QPoint (x0, yi - (yw/2));
			tr = QPoint (xi, yi - (yw/2));
			bl = QPoint (x0, yi + (yw/2));
			br = QPoint (xi, yi + (yw/2));
		} else {
			xi += xd;

			tl = QPoint (xi - (xw/2), yi);
			tr = QPoint (xi + (xw/2), yi);
			bl = QPoint (xi - (xw/2), y0);
			br = QPoint (xi + (xw/2), y0);
		}
		bar = QRect(tl, br).normalized();

		if ((bar.width() <= 2) || (bar.height() <= 2)) {
			QwtPainter::drawRect (p, bar);
		} else {
			p->setRenderHints (QPainter::Antialiasing|QPainter::HighQualityAntialiasing, false);

			p->setBrush (c); p->setPen (Qt::NoPen);
			QwtPainter::drawRect (p, bar.left() + 2, bar.top() + 2, bar.width() - 4, bar.height() - 4);

			p->setBrush (Qt::NoBrush);

			p->setPen (QPen(lc, 1));
			QwtPainter::drawLine (p, bar.left() + 0, bar.top() + 0, bar.left() + 0, bar.bottom() + 0);
			QwtPainter::drawLine (p, bar.left() + 1, bar.top() + 0, bar.left() + 1, bar.bottom() - 1);
			QwtPainter::drawLine (p, bar.left() + 2, bar.top() + 0, bar.right() + 0, bar.top() + 0);
			QwtPainter::drawLine (p, bar.left() + 2, bar.top() + 1, bar.right() - 1, bar.top() + 1);

			p->setPen (QPen(dc, 1));
			QwtPainter::drawLine (p, bar.right() + 0, bar.top() + 1, bar.right() + 0, bar.bottom() + 0);
			QwtPainter::drawLine (p, bar.right() - 1, bar.top() + 2, bar.right() - 1, bar.bottom() + 0);
			QwtPainter::drawLine (p, bar.left() + 1, bar.bottom() + 0, bar.right() - 2, bar.bottom() + 0);
			QwtPainter::drawLine (p, bar.left() + 2, bar.bottom() - 1, bar.right() - 2, bar.bottom() - 1);
		}
	}

	p->restore();
}

//void HistogramItem::draw(QPainter *painter, const QwtScaleMap &xMap,
//    const QwtScaleMap &yMap, const QRect &) const
//{
//    const QwtIntervalData &iData = d_data->data;
//
//    painter->setPen(QPen(d_data->color));
//
//    const int x0 = xMap.transform(baseline());
//    const int y0 = yMap.transform(baseline());
//
//    for ( int i = 0; i < (int)iData.size(); i++ )
//    {
//        if ( d_data->attributes & HistogramItem::Xfy )
//        {
//            const int x2 = xMap.transform(iData.value(i));
//            if ( x2 == x0 )
//                continue;
//
//            int y1 = yMap.transform( iData.interval(i).minValue());
//            int y2 = yMap.transform( iData.interval(i).maxValue());
//            if ( y1 > y2 )
//                qSwap(y1, y2);
//
//            if ( i < (int)iData.size() - 2 )
//            {
//                const int yy1 = yMap.transform(iData.interval(i+1).minValue());
//                const int yy2 = yMap.transform(iData.interval(i+1).maxValue());
//
//                if ( y2 == qwtMin(yy1, yy2) )
//                {
//                    const int xx2 = xMap.transform(
//                        iData.interval(i+1).minValue());
//                    if ( xx2 != x0 && ( (xx2 < x0 && x2 < x0) ||
//                                          (xx2 > x0 && x2 > x0) ) )
//                    {
//                       // One pixel distance between neighboured bars
//                       y2++;
//                    }
//                }
//            }
//
//            drawBar(painter, Qt::Horizontal,
//                QRect(x0, y1, x2 - x0, y2 - y1));
//        }
//        else
//        {
//            const int y2 = yMap.transform(iData.value(i));
//            if ( y2 == y0 )
//                continue;
//
//            int x1 = xMap.transform(iData.interval(i).minValue());
//            int x2 = xMap.transform(iData.interval(i).maxValue());
//            if ( x1 > x2 )
//                qSwap(x1, x2);
//
//            if ( i < (int)iData.size() - 2 )
//            {
//                const int xx1 = xMap.transform(iData.interval(i+1).minValue());
//                const int xx2 = xMap.transform(iData.interval(i+1).maxValue());
//
//                if ( x2 == qwtMin(xx1, xx2) )
//                {
//                    const int yy2 = yMap.transform(iData.value(i+1));
//                    if ( yy2 != y0 && ( (yy2 < y0 && y2 < y0) ||
//                                    (yy2 > y0 && y2 > y0) ) )
//                    {
//                        // One pixel distance between neighboured bars
//                        x2--;
//                    }
//                }
//            }
//            drawBar(painter, Qt::Vertical,
//                QRect(x1, y0, x2 - x1, y2 - y0) );
//        }
//    }
//}
