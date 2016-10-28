/** \file
 * The SPWaW war cabinet - plot handling - markers.
 *
 * Copyright (C) 2005-2016 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "resource.h"
#include "plot_mark.h"
#include "plot_private.h"

#define	PLOT_DW	12
#define	PLOT_DH	 8

PlotMark::PlotMark (int spacing)
	: QwtPlotMarker ()
{
	QColor	pc, bc;
	QPen	p;
	QBrush	b;

	DBG_TRACE_CONSTRUCT;

	/* Initialize */
	memset (&d, 0, sizeof (d));

	pc = *RES_color (RID_PLOT_MARK_FG);
	p.setColor (pc);

	bc = *RES_color (RID_PLOT_MARK_BG);
	b.setStyle (Qt::SolidPattern);
	b.setColor (bc);

	GUINEW (d.text, QwtText ("-", QwtText::PlainText), ERR_GUI_PLOT_INIT_FAILED, "mark: text");
	d.text->setBackgroundPen (p);
	d.text->setBackgroundBrush (b);

	setValue (0.0, 0.0);
	setSpacing (spacing);
	setZ (PLOT_Z_MARK);

	refresh();

	SET_GUICLS_NOERR;
}

PlotMark::~PlotMark (void)
{
	DBG_TRACE_DESTRUCT;

	delete d.text;
}

void
PlotMark::setText (QString &s)
{
	d.text->setText (s, QwtText::PlainText);
	refresh();
}

void
PlotMark::setAlign (Qt::Alignment align)
{
	setLabelAlignment (align);
}

void
PlotMark::refresh (void)
{
	DBG_TRACE_FENTER;

	if (!d.text) setLabel (QwtText ("mark")); else setLabel (*d.text);

	DBG_TRACE_FLEAVE;
}

void
PlotMark::drawme (QPainter *p, const QRect &/*c*/, const QPoint &xy) const
{
	int	align, pw, D, Dx, Dy;
	QPoint	trpos;
	QSize	size;
	QRect	rect;

	align = labelAlignment();
	trpos = xy;

	if ((pw = d.text->backgroundPen().width()) == 0) pw = 1;
	D = (pw+1)/2;

	Dx = QwtPainter::metricsMap().screenToLayoutX(spacing()) + D;
	Dy = QwtPainter::metricsMap().screenToLayoutY(spacing()) + D;

	size = d.text->textSize(p->font()) + QSize(PLOT_DW, PLOT_DH);
	rect.setRect (0, 0, size.width(), size.height());

	if (align & Qt::AlignLeft) {
		trpos.rx() -= (Dx + size.width());
	} else if (align & Qt::AlignRight) {
		trpos.rx() += (Dx);
	} else {
	        trpos.rx() -= (size.width()/2);
	}

	if (align & Qt::AlignTop) {
		trpos.ry() -= (Dy + size.height());
	} else if (align & Qt::AlignBottom) {
		trpos.ry() += (Dy);
	} else {
		trpos.ry() -= (size.height()/2);
	}

	p->translate (trpos.x(), trpos.y());
	d.text->draw(p, rect);
}

void
PlotMark::draw (QPainter *p, const QwtScaleMap &x, const QwtScaleMap &y, const QRect &c) const
{
	const int tx = x.transform(xValue());
	const int ty = y.transform(yValue());

	PlotMark::drawme (p, c, QPoint(tx, ty));
}

