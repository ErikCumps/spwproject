/** \file
 * The SPWaW war cabinet - GUI - progress dialog box.
 *
 * Copyright (C) 2005-2020 Erik Cumps <erik.cumps@gmail.com>
 *
 * License: GPL v2
 */

#include "../common.h"
#include "gui_progress.h"
#include "gui_private.h"

#define	DEF_MIN		0
#define	DEF_MAX		100

#define	MIN_DURATION	500	/* in ms */

GuiProgress::GuiProgress (const QString &label, const QString &cancel, bool mustshow, QWidget *P, Qt::WindowFlags F)
	: QProgressDialog (label, cancel, DEF_MIN, DEF_MAX, P, F)
{
	/* Initialize */
	memset (&d, 0, sizeof (d));
	d.active = GUI_WIN && GUI_WIN->isVisible();

	if (d.active) {
		/* Set progress dialog options */
		setWindowModality(Qt::WindowModal);
		setMinimumDuration (mustshow ? 0 : MIN_DURATION);

		d.min = DEF_MIN; d.max = DEF_MAX;
		setValue (d.v = d.min);
	} else {
		setHidden (true);
	}
}

void
GuiProgress::setRange (int min, int max)
{
	if (!d.active) return;

	QProgressDialog::setRange (min, max);

	d.min = min; d.max = max;
	setValue (d.v = d.min);
}

void
GuiProgress::inc (int step)
{
	int	n;

	if (!d.active) return;

	n = d.v + step;
	if (n > d.max) n = d.max;
	if (n < d.min) n = d.min;

	DBG_log ("[%s] %d <= v:%d <= %d\n", __FUNCTION__, d.min, n, d.max);

	GUI_FIXME;

	setValue (d.v = n);

	GUI_FIXME;
}

void
GuiProgress::dec (int step)
{
	int	n;

	if (!d.active) return;

	n = d.v - step;
	if (n > d.max) n = d.max;
	if (n < d.min) n = d.min;

	DBG_log ("[%s] %d <= v:%d <= %d\n", __FUNCTION__, d.min, n, d.max);

	GUI_FIXME;

	setValue (d.v = n);

	GUI_FIXME;
}

void
GuiProgress::done (void)
{
	DBG_log ("[%s] DONE, start\n", __FUNCTION__);

	if (!d.active) return;

	GUI_FIXME;

	DBG_log ("[%s] DONE, done\n", __FUNCTION__);

	setValue (d.v = d.max);

	GUI_FIXME;
}
